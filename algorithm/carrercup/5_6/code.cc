// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-04 17:57:04
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace NB {

int Swap(int a) {
  return ((a & 0x55555555) << 1) | ((a >> 1) & 0x55555555);
}

int Swap2(int a) {
  return ((a & 0x55555555) << 1) | ((a & 0xaaaaaaaa) >> 1);
}

}  // namespace NB

namespace algorithm {
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
