// Copyright 2014. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2014-07-20 20:12:29
// File  : file_binary.h
// Brief :

#ifndef  __FILE_BINARY_H_
#define  __FILE_BINARY_H_

#include <stdio.h>
#include <fstream>
#include "base/public/logging.h"
#include "base/public/basictypes.h"

namespace file {
class FileBinary {
 public:
  static bool ReadFileToString(const std::string & path,
                               std::string * rs);
  static bool WriteStringToFile(const std::string & content,
                                const std::string & path);
 private:
  DISALLOW_COPY_AND_ASSIGN(FileBinary);
};
};
#endif  //  __FILE_BINARY_H_
