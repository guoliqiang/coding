// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: wangbin@yunrang.com (bin wang)

#ifndef FILE_FILE_POSIX_H_
#define FILE_FILE_POSIX_H_

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <unistd.h>
#include <errno.h>

#include <vector>
#include <string>

#include "../public/file_base.h"
#include "../../base/public/string_util.h"
#include "../../third_part/registerer/public/registerer.h"

namespace file {

class FilePosix: public FileBase {
 public:

  FilePosix();

  ~FilePosix();

  Status Write(const void *buffer, size_type length);

  Status Read(size_type length, std::string *result);

  Status Seek(size_type pos, SeekPos seek_pos);

  size_type Position() const;

  Status Flush();

  bool Eof() const;

  static bool Exists(const std::string &path);

  static bool IsDir(const std::string &path);

  static Status MoveFile(const std::string &old_path,
                         const std::string &new_path);

  static Status GetDirsInDir(const std::string &dir,
                             std::vector<std::string> *dirs);

  static Status GetFilesInDir(const std::string &dir,
                              std::vector<std::string> *files);

  static Status CreateDir(const std::string &path);

  static Status DeleteRecursively(const std::string& name);

 protected:
  Status OpenInternal(const std::string &path, Mode mode);

  bool IsSeekable() const;

  // Join the path.
  static std::string JoinPath(const std::string &dirname,
                              const std::string &basename);

 private:
  FILE *file_;
  size_type pos_;

  DISALLOW_COPY_AND_ASSIGN(FilePosix);
};

}  // namespace file

#endif  // FILE_FILE_POSIX_H_
