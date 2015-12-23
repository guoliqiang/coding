// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-22 12:34:26
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
template <typename T>

void Destory(T * ptr) {
  LOG(INFO) << "b";
  ptr->~T();
  LOG(INFO) << "e";
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  int t = 10;
  int * p = new(&t)int(100);
  LOG(INFO) << *p << " " << t;
  Destory(p);
  return 0;
}
