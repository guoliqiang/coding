// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-05 14:31:00
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int Remove(char * s) {
  char * r = s;
  char * w = s;
  while (*r != '\0') {
    *w = *r;
    w++;
    if (*r == '/') {
      char * t = r;
      while (*t == '/') t++;
      r = t;
    } else r++;
  }
  *w = '\0';
  return w - s;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  char tmp[100] = "/root//foo/bar";
  Remove(tmp);
  LOG(INFO) << tmp;
  return 0;
}
