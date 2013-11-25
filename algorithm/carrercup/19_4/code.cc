// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 11:31:16
// File  : code.cc
// Brief :

/*
 * Write a method which finds the maximum of two numbers. You should not use if-else or any other comparison operator. 
 * */

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
  int k = ((t >> 31) & 1);  // 必须& 1因为负数右移前面会补1
  return a - k * t;
}

int max2(int a, int b) {
  int c[2] = {a, b};
  int k = ((a - b) >> 31) & 1;
  return c[k];
}
}  // namespace NB

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << NB::max(7, 8) << " " << NB::max(10, 7);
  LOG(INFO) << NB::max2(7, 8) << " " << NB::max2(10, 7);
  return 0;
}
