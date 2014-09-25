// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-02-04 16:29:29
// File  : code.cc
// Brief : 从indri的代码中提取出的hashtable，可以支持内存池

#include "base/public/common_ojhead.h"
#include "HashTable.h"

namespace algorithm {
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  indri::utility::HashTable<std::string, std::string> ht;
  return 0;
}
