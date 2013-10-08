// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 10:05:59
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

template<typename T>
void Swap(T & a, T & b) {
  a ^= b;
  b ^= a;
  a ^= b;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int ia = 10;
  int ib = 3;
  LOG(INFO) << ia << " " << ib;
  Swap(ia, ib);
  LOG(INFO) << ia << " " << ib;
  return 0;
}
