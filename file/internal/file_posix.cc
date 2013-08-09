// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: wangbin@yunrang.com (bin wang)

#include <errno.h>

#include "../public/file_posix.h"
#include "base/public/scoped_ptr.h"
#include "base/public/logging.h"

using std::string;
using std::vector;

namespace file {

FilePosix::FilePosix(): FileBase(), file_(NULL), pos_(0) {}

FilePosix::~FilePosix() {
  if (file_ != NULL) {
    fclose(file_);
  }
}

// TODO(someone): set proper error code
Status FilePosix::OpenInternal(const string &path, Mode mode) {
  CHECK(file_ == NULL) << "the file is already opened.";
  string mode_str;
  switch (mode) {
    case kAppend:
      mode_str = "a+";
      break;
    case kWrite:
      mode_str = "w+";
      break;
    case kRead:
      mode_str = "r";
      break;
    default:
      DCHECK(false) << "invalid mode type: " << mode;
  }
  file_ = fopen(path.c_str(), mode_str.c_str());
  return file_ ? Status::OK() : Status::IOError(StringPrintf(
        "path %s with mode %s", path.c_str(), mode_str.c_str()));
}

bool FilePosix::IsSeekable() const {
  return true;
}

Status FilePosix::Seek(size_type pos, SeekPos seek_pos) {
  if (fseek(file_, pos, seek_pos)) {
    LOG(ERROR) << "file seek error!";
    return Status::OK();
  }
  return Status::OK();
}

FileBase::size_type FilePosix::Position() const {
  return ftell(file_);
}

bool FilePosix::Eof() const {
  return feof(file_);
}

Status FilePosix::Write(const void *buffer, size_type length) {
  size_type size = fwrite(buffer, 1, length, file_);
  return (size == length) ? Status::OK() : Status::IOError("write fail");
}

Status FilePosix::Read(size_type length, std::string *result) {
  result->clear();
  scoped_array<char> buffer(new char[length + 1]);
  size_type size = fread(buffer.get(), 1, length, file_);
  result->assign(buffer.get(), size);
  if (size == length || feof(file_)) {
    return Status::OK();
  } else {
    int err_no = ferror(file_);
    if (err_no == ENXIO || err_no == ENOMEM) {
      return Status::Corruption("device error");
    } else {
      return Status::IOError("read faile");
    }
  }
  return Status::OK();
}

Status FilePosix::Flush() {
  return fflush(file_) == 0 ? Status::OK() : Status::IOError("flush fail");
}

bool FilePosix::Exists(const string& path) {
  return access(path.c_str(), F_OK) == 0;
}

bool FilePosix::IsDir(const std::string& path) {
  if (!Exists(path))
    return false;
  struct stat statbuf;
  lstat(path.c_str(), &statbuf);
  return S_ISDIR(statbuf.st_mode);
}

Status FilePosix::MoveFile(const std::string &old_path,
                           const std::string &new_path) {
  if (rename(old_path.c_str(), new_path.c_str())) {
    string msg = strerror(errno);
    CHECK(errno != EXDEV) << "Invalid cross-device link";
    return Status::IOError(msg);
  }
  return Status::OK();
}

Status FilePosix::CreateDir(const string& path) {
  return mkdir(path.c_str(), 0755) == 0 ? Status::OK() : Status::IOError(path);
}

Status FilePosix::GetFilesInDir(const string& dir, vector<string>* files) {
  DIR *dp = NULL;
  struct dirent entry;
  struct dirent* entryPtr = NULL;
  vector<string> result;

  if ((dp = opendir(dir.c_str())) == NULL) {
    return Status::IOError("cannot open directory: " + dir);
  }

  readdir_r(dp, &entry, &entryPtr);
  while (entryPtr != NULL) {
    const string path = JoinPath(dir, entry.d_name);
    readdir_r(dp, &entry, &entryPtr);
    if (IsDir(path))
      continue;
    result.push_back(path);
  }
  closedir(dp);
  files->swap(result);
  return Status::OK();
}

Status FilePosix::GetDirsInDir(const std::string& dir,
                               vector<std::string>* dirs) {
  DIR *dp = NULL;
  struct dirent entry;
  struct dirent* entryPtr = NULL;
  vector<string> result;

  if ((dp = opendir(dir.c_str())) == NULL) {
    return Status::IOError("cannot open directory: " + dir);
  }

  readdir_r(dp, &entry, &entryPtr);
  while (entryPtr != NULL) {
    if (0 == strcmp(".", entry.d_name) ||
        0 == strcmp("..", entry.d_name)) {
      readdir_r(dp, &entry, &entryPtr);
      continue;
    }
    const string path = JoinPath(dir, entry.d_name);
    readdir_r(dp, &entry, &entryPtr);
    if (!IsDir(path))
      continue;
    result.push_back(path);
  }
  closedir(dp);
  dirs->swap(result);
  return Status::OK();
}

// TODO(someone): set proper error code
Status FilePosix::DeleteRecursively(const string& name) {
  // We don't care too much about error checking here since this is only used
  // in tests to delete temporary directories that are under /tmp anyway.
  struct stat stats;
  if (lstat(name.c_str(), &stats) != 0)
    return Status::IOError(name);

  Status result;
  if (S_ISDIR(stats.st_mode)) {
    DIR* dir = opendir(name.c_str());
    if (dir != NULL) {
      while (true) {
        struct dirent entry;
        struct dirent* entryPtr = NULL;
        readdir_r(dir, &entry, &entryPtr);
        if (entryPtr == NULL) break;
        string entry_name = entry.d_name;
        if (entry_name != "." && entry_name != "..") {
          Status s = DeleteRecursively(name + "/" + entry_name);
          if (!s.ok())
            result = s;
        }
      }
    }
    closedir(dir);
    rmdir(name.c_str());
  } else if (S_ISREG(stats.st_mode)) {
    remove(name.c_str());
  }
  return result;
}

string FilePosix::JoinPath(const string& dirname, const string& basename) {
  if (StartsWithASCII(basename, "/", false))
    return basename;
  string result = dirname;
  if (!EndsWith(dirname, "/", false))
    result += "/";
  return result + basename;
}

REGISTER_FILEBASE(FilePosix);
}
