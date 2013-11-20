// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-19 22:17:52
// File  : code.cc
// Brief :

/*
 * Accepted  312K 16MS
 * 原理分析见 number/gcd/
 * */

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
// 这道题不能用，因为不一定互质
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
    long long a1, r1, a2, r2;
    scanf("%lld%lld", &a1, &r1);
    getchar();
    int ok = 0;
    for (int i = 0; i < n - 1; i++) {
      scanf("%lld%lld", &a2, &r2);
      getchar();
      if (ok) continue;
      long long x, y;
      long long g = exGcd(a1, a2, x, y);
      long long c = r2 - r1;
      if (c % g != 0) {
        ok = 1;
        continue;
      }
      int q = a2 / g;
      x = (x * c/g % q + q) % q;
      r1 = a1 * x + r1;  // r1 = 结果
      a1 = a1 * a2 / g;  
    }
    if (ok) printf("-1\n");
    else printf("%lld\n", r1);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
