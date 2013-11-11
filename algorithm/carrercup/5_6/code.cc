// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-04 17:57:04
// File  : code.cc
// Brief :

/*
 * Write a program to swap odd and even bits in an integer with as few instructions as possible
 * (e.g., bit 0 and bit 1 are swapped, bit 2 and bit 3 are swapped, etc).
 * */

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
