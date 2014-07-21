// Copyright 2014 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2014-07-20 20:20:55
// File  : file_binary.cc
// Brief :

#include <string>
#include "file/public/file_binary.h"

namespace file {
// static
bool FileBinary::ReadFileToString(const std::string & path,
                                  std::string * rs) {
  std::ifstream fin;
  fin.open(path.c_str(), std::ios::binary);
  if (fin == NULL) {
    LOG(ERROR) << "open " << path << " error!";
    return false;
  }
  fin.seekg(0, std::ios::end);
  rs->resize(fin.tellg());
  fin.seekg(0, std::ios::beg);
  fin.read(&(rs->at(0)), rs->size());
  fin.close();
  return true;
}
// static
bool FileBinary::WriteStringToFile(const std::string & content,
                                   const std::string & path) {
  std::ofstream fout;
  fout.open(path.c_str(), std::ios::binary);
  if (fout == NULL) {
    LOG(ERROR) << "open " << path << " error!";
    return false;
  }
  fout.write(content.c_str(), content.size());
  fout.close();
  return true;
}

}  // namespace file
