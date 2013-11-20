// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-20 01:56:50
// File  : code.cc
// Brief :

/*
 * Accepted  132K 0MS
 *
 * polya 定理
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 25;
long long p[MAXN];

int Gcd(int a, int b) {
  if (!a) return b;
  if (!b) return a;
  return Gcd(b, a % b);
}

void Cal() {
  p[0] = 1;
  for (int i = 1; i < MAXN; i++) {
    p[i] = p[i - 1] * 3;
  }
}

long long Polya(int k) {
  // 翻转情况
  long long rs = k & 1 ? p[k / 2 + 1] * k : (p[k / 2 + 1] + p[k / 2]) * k / 2;
  // 旋转情况
  for (int i = 1; i <= k; i++) {
    int t = Gcd(i, k);
    rs += p[t];
  }
  rs /= 2 * k;
  return rs;
}

void Read() {
  memset(p, 0, sizeof(p));
  Cal();
  int n = 0;
  while (scanf("%d", &n) && n != -1) {
    if (n == 0) printf("0\n");
    else {
      long long t = Polya(n);
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
