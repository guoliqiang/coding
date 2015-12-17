// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-17 22:37:29
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int HammingWeight(uint32_t n) {
  uint32_t one = 0x55555555;
  uint32_t two = 0x33333333;
  uint32_t four = 0x0f0f0f0f;
  uint32_t eight = 0x00ff00ff;
  uint32_t sixteen = 0x0000ffff;

  n = (n & one) + ((n & (one << 1)) >> 1);
  n = (n & two) + ((n & (two << 2)) >> 2);
  n = (n & four) + ((n & (four << 4)) >> 4);
  n = (n & eight) + ((n & (eight << 8)) >> 8);
  n = (n & sixteen) + ((n & (sixteen << 16)) >> 16);
  return n;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << HammingWeight(15);
  return 0;
}
