// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-04 14:07:57
// File  : code.cc
// Brief :

/*
 * 一个有符号数，如果它的最高位为1，它右移若干位后到达位置i，
 * 那么最高位到第i位之间全是1.
 *
 * 个无符号数，如果它的最高位为1，它右移若干位后到达位置i，
 * 那么最高位到第i位之间全是0
 *
 * http://hawstein.com/posts/5.1.html
 *
 * */
#include "base/public/common_head.h"

namespace algorithm {

void Convert(int m, int i, int j, int * n) {
   i = std::min(31, std::max(i, 0));
   j = std::min(31, std::max(j, 0));
   if (i > j) std::swap(i, j);
   int t = (1 << j) + ((1 << j) - 1) -  ((1 << i) - 1);
   *n = (*n) ^ (t & (*n)) ^ (t & m);  // bug fixed
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int m = 0x9c;
  int n = 1;
  Convert(m, 1, 32, &n);
  LOG(INFO) << n;
  return 0;
}
