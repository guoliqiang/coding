// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-21 00:22:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
long long C(long long n, long long k) {
  k = std::min(n - k, k);  // 0ms
  long long j = 2;
  long long rs = 1;
  for (int i = 0; i < k; i++) {
    rs *= (n - i);
    while (j <= k && rs % j == 0) {
      rs /= j;
      j++;
    }
  }
  return rs;
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF) {
    long long t = C(2 * n, n) / (n + 1);
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
