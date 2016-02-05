// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-17 21:59:48
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

uint32_t ReverseBits(uint32_t n) {
  uint32_t one = 0x55555555;
  uint32_t two = 0x33333333;
  uint32_t four = 0x0f0f0f0f;
  uint32_t eight = 0x00ff00ff;
  uint32_t sixteen = 0x0000ffff;

  n = ((n & one) << 1) | ((n & (one << 1)) >> 1);
  n = ((n & two) << 2) | ((n & (two << 2)) >> 2);
  n = ((n & four) << 4) | ((n & (four << 4)) >> 4);
  n = ((n & eight) << 8) | ((n & (eight << 8)) >> 8);
  n = ((n & sixteen) << 16) | ((n & (sixteen << 16)) >> 16);
  return n;
}
}  // namespace algorithm

using namespace algorithm;

namespace clear {
uint32_t ReverseBits(uint32_t n) {
    uint32_t ans = 0;
    for (int i = 0; i < 32; i++) {
        ans += (n & 1);
        if (i == 31) break;
        ans <<= 1;
        n >>= 1;
    }
    return ans;
}
}  // namespace clear

int main(int argc, char** argv) {
  LOG(INFO) << clear::ReverseBits(1);
  return 0;
}
