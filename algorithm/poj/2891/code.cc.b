// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-19 22:17:52
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 10003;
int N = 0;
long long a[MAXN];
long long  r[MAXN];

// y is smaller
long long Gcd(long long x, long long y) {
  if (!x) return y;
  if (!y) return x;
  return Gcd(y, x % y);
}

long long VGcd(long long k) {
  if (k == N - 1) return a[k];
  return Gcd(a[k], VGcd(k + 1));
}

bool Check() {
  if (N == 0) return false;
  return VGcd(0) == 1;
}

long long exGcd(long long a, long long b, long long &x, long long &y) {
  if (!b) {
    x = 1;
    y = 0;
    return a;
  } else {
    long long rs = exGcd(b, a % b, x, y);
    long long t = x;
    x = y;
    y = t - (a / b) * y;
    return rs;
  }
}

// 对于a两两互质的情况才可以
long long Min() {
  long long t = 1;
  for (int i = 0; i < N; i++) t *= a[i];
  long long rs = 0;
  for (int i = 0; i < N; i++) {
    long long cur = t / a[i];
    long long x, y;
    exGcd(cur, a[i], x, y);
    // LOG(INFO) << x << "*" << cur << " + " << a[i] << "*" << y;
    rs = (rs + cur * x * r[i]) % t;
  }
  while (rs < 0) rs += t;
  return rs;
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF) {
    getchar();
    memset(a, 0, sizeof(a));
    memset(r, 0, sizeof(r));
    N = n;
    for (int i = 0; i < n; i++) {
      long long x, y;
      scanf("%lld%lld", &x, &y);
      getchar();
      a[i] = x;
      r[i] = y;
    }
    if (Check() == false) {
      printf("-1\n");
    } else {
      long long t = Min();
      printf("%lld\n", t);
    }
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
