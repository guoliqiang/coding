// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 23:35:05
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 28 milli secs
 * Progress: 299/299 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 39/39 test cases passed.
 *
 * */

#include <algorithm>
#include <math.h>
#include "base/public/logging.h"

namespace algorithm {

double Pow(double x, int n) {
  if (n == 0) return 1;
  long long int k = n;
  if (n < 0) k *= -1;

  double rs = 1;
  double foo = x;
  while (k) {
    if (k & 0x1) rs *= foo;
    foo *= foo;
    k >>= 1;
  }
  return n >0 ? rs : 1.0 / rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Pow(-1, -2147483648);
  // 负数取反后越界
  int foo = -2147483648;
  LOG(INFO) << foo << " " << foo * -1;
  // abs 得到的依旧是负数，fabs得到的是正数
  LOG(INFO) << abs(foo) << " " << fabs(foo);
  return 0;
}
