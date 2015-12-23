// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-16 23:21:58
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  double t = 1.34;
  int i = reinterpret_cast<int &>(t); // equal with int i = *(reinterpret_cast<int *>(&t));
  int j = static_cast<int>(t);
  LOG(INFO) << i << " " << j;
  return 0;
}
