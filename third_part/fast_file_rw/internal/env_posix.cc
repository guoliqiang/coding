// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <deque>
#include <set>
#include "third_part/fast_file_rw/public/env.h"
#include "base/public/atomic_pointer.h"
#include "base/public/light_singleton.h"
#include "file/public/file.h"

namespace fast_file_rw {

static Status IOError(const std::string& context, int err_number) {
  return Status::IOError(context, strerror(err_number));
}

static Status GetFileSize(const std::string & file_name, uint64_t * size) {
  Status s;
  if (file::File::FileSize(file_name, size)) {
    return s;
  } else {
    return IOError(file_name, errno);
  }
}

class PosixSequentialFile: public SequentialFile {
 public:
  PosixSequentialFile(const std::string& fname, FILE* f)
      : filename_(fname), file_(f) { }
  virtual ~PosixSequentialFile() {
    fclose(file_);
  }

  virtual Status Read(size_t n, std::string* result, char* scratch) {
    Status s;
    size_t r = fread_unlocked(scratch, 1, n, file_);
    *result = std::string(scratch, r);
    if (r < n) {
      if (feof(file_)) {
        // We leave status as ok if we hit the end of the file
      } else {
        // A partial read with an error: return a non-ok status
        s = IOError(filename_, errno);
      }
    }
    return s;
  }

  virtual Status Skip(uint64_t n) {
    if (fseek(file_, n, SEEK_CUR)) {
      return IOError(filename_, errno);
    }
    return Status::OK();
  }

 private:
  std::string filename_;
  FILE * file_;
};

// pread() based random-access
// http://www.cnblogs.com/brill/p/3226439.html
class PosixRandomAccessFile: public RandomAccessFile {
 public:
  PosixRandomAccessFile(const std::string& fname, int fd)
      : filename_(fname), fd_(fd) { }
  virtual ~PosixRandomAccessFile() {
    close(fd_);
  }

  virtual Status Read(uint64_t offset, size_t n, std::string* result,
                      char* scratch) const {
    Status s;
    ssize_t r = pread(fd_, scratch, n, static_cast<off_t>(offset));
    *result = std::string(scratch, (r < 0) ? 0 : r);
    if (r < 0) {
      // An error: return a non-ok status
      s = IOError(filename_, errno);
    }
    return s;
  }

 private:
  std::string filename_;
  int fd_;
};

// Helper class to limit mmap file usage so that we do not end up
// running out virtual memory or running into kernel performance
// problems for very large databases.
class MmapLimiter {
 public:
  // Up to 1000 mmaps for 64-bit binaries; none for smaller pointer sizes.
  MmapLimiter() {
    SetAllowed(sizeof(void*) >= 8 ? 1000 : 0);  // NOLINT
  }

  // If another mmap slot is available, acquire it and return true.
  // Else return false.
  bool Acquire() {
    if (GetAllowed() <= 0) {
      return false;
    }
    base::MutexLock l(&mu_);
    intptr_t x = GetAllowed();
    if (x <= 0) {
      return false;
    } else {
      SetAllowed(x - 1);
      return true;
    }
  }

  // Release a slot acquired by a previous call to Acquire() that returned true.
  void Release() {
    base::MutexLock l(&mu_);
    SetAllowed(GetAllowed() + 1);
  }

 private:
  base::Mutex mu_;
  base::AtomicPointer allowed_;

  intptr_t GetAllowed() const {
    return reinterpret_cast<intptr_t>(allowed_.Acquire_Load());
  }

  // REQUIRES: mu_ must be held
  void SetAllowed(intptr_t v) {
    allowed_.Release_Store(reinterpret_cast<void*>(v));
  }

  DISALLOW_COPY_AND_ASSIGN(MmapLimiter);
};

// mmap() based sequential-access
class PosixMmapSequentialReadableFile: public SequentialFile {
 public:
  // base[0,length-1] contains the mmapped contents of the file.
  PosixMmapSequentialReadableFile(const std::string& fname, void* base,
                                  size_t length, MmapLimiter* limiter)
      : filename_(fname), mmapped_region_(base), length_(length),
        left_size_(length), limiter_(limiter) {
  }

  virtual ~PosixMmapSequentialReadableFile() {
    munmap(mmapped_region_, length_);
    limiter_->Release();
  }

  virtual Status Read(size_t n, std::string* result,
                      char* scratch) {
    Status s;
    if (left_size_ <= 0) {
      *result = std::string();
    } else {
      int cur_size = std::min(left_size_, n);
      *result = std::string(reinterpret_cast<char*>(mmapped_region_), cur_size);
      mmapped_region_ = reinterpret_cast<char*>(mmapped_region_) + cur_size;
      left_size_ -= cur_size;
    }
    return s;
  }

  virtual Status Skip(uint64_t n) {
    if (left_size_ <  n)  n = left_size_;
    mmapped_region_ = reinterpret_cast<char*>(mmapped_region_) + n;
    left_size_ -= n;
    return Status::OK();
  }

 private:
  std::string filename_;
  void * mmapped_region_;
  size_t length_;
  size_t left_size_;
  MmapLimiter* limiter_;
};

// mmap() based random-access
class PosixMmapReadableFile: public RandomAccessFile {
 public:
  // base[0,length-1] contains the mmapped contents of the file.
  PosixMmapReadableFile(const std::string& fname, void* base, size_t length,
                        MmapLimiter* limiter)
      : filename_(fname), mmapped_region_(base), length_(length),
        limiter_(limiter) {
  }

  virtual ~PosixMmapReadableFile() {
    munmap(mmapped_region_, length_);
    limiter_->Release();
  }

  virtual Status Read(uint64_t offset, size_t n, std::string* result,
                      char* scratch) const {
    Status s;
    if (offset >= length_) {
      *result = std::string();
      s = IOError(filename_, EINVAL);
    } else {
      int len = std::min(n, length_ - offset);
      *result = std::string(reinterpret_cast<char*>(mmapped_region_) + offset,
                            len);
    }
    return s;
  }

 private:
  std::string filename_;
  void* mmapped_region_;
  size_t length_;
  MmapLimiter* limiter_;
};

// We preallocate up to an extra megabyte and use memcpy to append new
// data to the file.  This is safe since we either properly close the
// file before reading from it, or for log files, the reading code
// knows enough to skip zero suffixes.
class PosixMmapFile : public WritableFile {
 public:
  PosixMmapFile(const std::string& fname, int fd, size_t page_size)
      : filename_(fname),
        fd_(fd),
        page_size_(page_size),
        map_size_(Roundup(65536, page_size)),
        base_(NULL),
        limit_(NULL),
        dst_(NULL),
        last_sync_(NULL),
        file_offset_(0),
        pending_sync_(false) {
    CHECK_EQ((page_size & (page_size - 1)), 0);
  }

  ~PosixMmapFile() {
    if (fd_ >= 0) {
      PosixMmapFile::Close();
    }
  }

  virtual Status Append(const std::string& data) {
    const char* src = data.data();
    size_t left = data.size();
    while (left > 0) {
      CHECK(base_ <= dst_) << base_ << " " << dst_;
      CHECK(dst_ <= limit_) << dst_ << " " << limit_;
      size_t avail = limit_ - dst_;
      if (avail == 0) {
        if (!UnmapCurrentRegion() ||
            !MapNewRegion()) {
          return IOError(filename_, errno);
        }
      }

      size_t n = (left <= avail) ? left : avail;
      memcpy(dst_, src, n);
      dst_ += n;
      src += n;
      left -= n;
    }
    return Status::OK();
  }

  virtual Status Close() {
    Status s;
    size_t unused = limit_ - dst_;
    if (!UnmapCurrentRegion()) {
      s = IOError(filename_, errno);
    } else if (unused > 0) {
      // Trim the extra space at the end of the file
      if (ftruncate(fd_, file_offset_ - unused) < 0) {
        s = IOError(filename_, errno);
      }
    }

    if (close(fd_) < 0) {
      if (s.ok()) {
        s = IOError(filename_, errno);
      }
    }

    fd_ = -1;
    base_ = NULL;
    limit_ = NULL;
    return s;
  }

  virtual Status Flush() {
    return Sync();
  }

  // http://blog.csdn.net/cywosp/article/details/8767327
  virtual Status Sync() {
    Status s;
    if (pending_sync_) {
      // Some unmapped data was not synced
      pending_sync_ = false;
      if (fdatasync(fd_) < 0) {
        s = IOError(filename_, errno);
      }
    }

    if (dst_ > last_sync_) {
      // Find the beginnings of the pages that contain the first and last
      // bytes to be synced.
      size_t p1 = TruncateToPageBoundary(last_sync_ - base_);
      size_t p2 = TruncateToPageBoundary(dst_ - base_ - 1);
      last_sync_ = dst_;
      if (msync(base_ + p1, p2 - p1 + page_size_, MS_SYNC) < 0) {
        s = IOError(filename_, errno);
      }
    }

    return s;
  }

 private:
  std::string filename_;
  int fd_;
  size_t page_size_;
  size_t map_size_;       // How much extra memory to map at a time
  char* base_;            // The mapped region
  char* limit_;           // Limit of the mapped region
  char* dst_;             // Where to write next  (in range [base_,limit_])
  char* last_sync_;       // Where have we synced up to
  uint64_t file_offset_;  // Offset of base_ in file

  // Have we done an munmap of unsynced data?
  bool pending_sync_;

  // Roundup x to a multiple of y
  static size_t Roundup(size_t x, size_t y) {
    return ((x + y - 1) / y) * y;
  }

  size_t TruncateToPageBoundary(size_t s) {
    s -= (s & (page_size_ - 1));
    CHECK_EQ((s % page_size_), 0);
    return s;
  }

  bool UnmapCurrentRegion() {
    bool result = true;
    if (base_ != NULL) {
      if (last_sync_ < limit_) {
        // Defer syncing this data until next Sync() call, if any
        pending_sync_ = true;
      }
      if (munmap(base_, limit_ - base_) != 0) {
        result = false;
      }
      file_offset_ += limit_ - base_;
      base_ = NULL;
      limit_ = NULL;
      last_sync_ = NULL;
      dst_ = NULL;

      // Increase the amount we map the next time, but capped at 1MB
      if (map_size_ < (1<<20)) {
        map_size_ *= 2;
      }
    }
    return result;
  }

  bool MapNewRegion() {
    CHECK(base_ == NULL) << base_;
    // http://blog.sina.com.cn/s/blog_4b69e3010100q1cv.html
    if (ftruncate(fd_, file_offset_ + map_size_) < 0) {
      return false;
    }
    void* ptr = mmap(NULL, map_size_, PROT_READ | PROT_WRITE, MAP_SHARED,
                     fd_, file_offset_);
    if (ptr == MAP_FAILED) {
      return false;
    }
    base_ = reinterpret_cast<char*>(ptr);
    limit_ = base_ + map_size_;
    dst_ = base_;
    last_sync_ = base_;
    return true;
  }
};

class PosixEnv : public Env {
 public:
  PosixEnv();
  virtual ~PosixEnv() {}

  virtual Status NewSequentialFile(const std::string& fname,
                                   SequentialFile** result) {
    if (mmap_limit_.Acquire()) {
      Status s;
      int fd = open(fname.c_str(), O_RDONLY);
      if (fd < 0) {
        s = IOError(fname, errno);
      } else {
        uint64_t size;
        s = GetFileSize(fname, &size);
        if (s.ok()) {
          void* base = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
          if (base != MAP_FAILED) {
            *result = new PosixMmapSequentialReadableFile(fname, base,
                size, &mmap_limit_);
            if (madvise(base, size, MADV_WILLNEED | MADV_SEQUENTIAL) == -1) {
              s = IOError(fname, errno);
            }
          } else {
            s = IOError(fname, errno);
          }
        }
        close(fd);
      }
      if (!s.ok()) mmap_limit_.Release();
      return s;
    } else {
      FILE * f = fopen(fname.c_str(), "r");
      if (f == NULL) {
        *result = NULL;
        return IOError(fname, errno);
      } else {
        *result = new PosixSequentialFile(fname, f);
        return Status::OK();
      }
    }
  }

  virtual Status NewRandomAccessFile(const std::string& fname,
                                     RandomAccessFile** result) {
    *result = NULL;
    Status s;
    int fd = open(fname.c_str(), O_RDONLY);
    if (fd < 0) {
      s = IOError(fname, errno);
    } else if (mmap_limit_.Acquire()) {
      uint64_t size;
      s = GetFileSize(fname, &size);
      if (s.ok()) {
        void* base = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
        if (base != MAP_FAILED) {
          *result = new PosixMmapReadableFile(fname, base, size, &mmap_limit_);
          if (madvise(base, size, MADV_RANDOM) == -1) {
            s = IOError(fname, errno);
          }
        } else {
          s = IOError(fname, errno);
        }
      }
      close(fd);
      if (!s.ok()) {
        mmap_limit_.Release();
      }
    } else {
      *result = new PosixRandomAccessFile(fname, fd);
    }
    return s;
  }

  virtual Status NewWritableFile(const std::string& fname,
                                 WritableFile** result) {
    Status s;
    const int fd = open(fname.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0) {
      *result = NULL;
      s = IOError(fname, errno);
    } else {
      *result = new PosixMmapFile(fname, fd, page_size_);
    }
    return s;
  }

 private:
  size_t page_size_;
  MmapLimiter mmap_limit_;
};

PosixEnv::PosixEnv() : page_size_(getpagesize()) { }

Env * Env::Default() {
  return LightSingleton<PosixEnv>::GetInstance();
}

}  // namespace fast_file_rw
