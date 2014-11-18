// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-17 22:33:19
// File  : large_file_reader.h
// Brief :

#ifndef  __LARGE_FILER_READER_H_
#define  __LARGE_FILER_READER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "base/public/logging.h"
#include "file/public/file.h"

namespace large_file_reader {

class LargeFileReader {
 public:
  enum ReadType { SEQUENCE = 1, RANDOM = 2 };

 public:
  LargeFileReader(const std::string path, ReadType type = SEQUENCE) {
    path_ = path;
    CHECK(file::File::Exists(path_)) << "not find file:" << path_;
    file_len_ = 0;
    file::File::FileSize(path_, &file_len_);
    int fd = open(path_.c_str(), O_RDONLY);
    file_ptr_ = (char *) mmap(NULL, file_len_, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);

    CHECK(file_ptr_ != MAP_FAILED) << "run mmap for " << path_ << " error";
    if (type == SEQUENCE) {
      CHECK(madvise(file_ptr_, file_len_, MADV_WILLNEED | MADV_SEQUENTIAL)
            != -1) << "sequence madvise error for " << path_;
    } else {
      CHECK(madvise(file_ptr_, file_len_, MADV_RANDOM) != -1)
          << "random madvise error for " << path_;
    }
  }

  bool Read(int offset, int size, std::string * rs) {
    if (offset + size >= file_len_) {
      return false;
    }
    rs->resize(size);
    memmove((void *)rs->data(), (void *)(file_ptr_ + offset), size);
    return true;
  }

 private:
  std::string path_;
  char * file_ptr_;
  size_t file_len_;
};

}  // namespace large_file_reader

#endif  // __LARGE_FILER_READER_H_
