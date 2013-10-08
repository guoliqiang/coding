// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 11:31:16
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

int max(int a, int b) {
  return (a - b) & (1 << 31) ? b : a;
}

}  // namespace algorithm

namespace NB {
// 这个方法也很巧妙
int max(int a, int b) {
  int t = a - b;
  int k = t & (1 << 31);
  return a - k * t;
}

int max2(int a, int b) {
  int c[2] = {a, b};
  int k = (a - b) & (1 << 31);
  return c[k];
}
}  // namespace NB

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << max(7, 8) << " " << max(10, 7);
  return 0;
}
