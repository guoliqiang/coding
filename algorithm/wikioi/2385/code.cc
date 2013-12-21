// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-21 10:33:11
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
long long n;
long long Pow(long long x, long long y) {
  long long rs = 1;
  long long t = x;
  while (y) {
    if (y & 1) rs = (rs * t) % n;
    t = (t * t) % n;
    y >>= 1;
  }
  return rs;
}

void Read() {
  long long m, k, x;
  while (scanf("%lld%lld%lld%lld", &n, &m, &k, &x) != EOF) {
    getchar();
    long long t = (m * Pow(10, k)) % n;
    t = (x + t) % n;
    printf("%lld\n", t);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
