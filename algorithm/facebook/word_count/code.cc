// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-05 14:42:18
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int WordCount(const char * str) {
  char cur = ' ';
  char pre = ' ';
  int rs = 0;
  while (*str != '\0') {
    cur = *str;
    if (cur == ' ' && pre != ' ') rs++;
    pre = cur;
    str++;
  }
  if (cur != ' ') rs++;
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  LOG(INFO) << WordCount("  guo li  qiang");
  LOG(INFO) << WordCount("  guo li  qiang ");
  LOG(INFO) << WordCount("g u  o li  qiang ");
  return 0;
}
