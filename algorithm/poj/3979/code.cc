// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-19 20:51:35
// File  : code.cc
// Brief :

/*
 * Accepted  132K 0MS
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
int Gcd(int x, int y) {
  if (!x) return y;
  if (!y) return x;
  return Gcd(y % x, x);
}

int Lcm(int x, int y) {
  return (x * y) / Gcd(x, y);
}

void Read() {
  int a1, a2, b1, b2;
  bool add;
  while (scanf("%d", &a1) != EOF) {
    getchar();
    scanf("%d", &a2);
    char ch = getchar();
    if (ch == '+') add = true;
    else add = false;
    scanf("%d", &b1);
    getchar();
    scanf("%d", &b2);
    getchar();
    int t = Lcm(a2, b2);
    a1 = a1 * (t / a2);
    b1 = b1 * (t / b2);
    
    if (add) a1 = a1 + b1;
    else a1 = a1 - b1;
    if (a1 == 0 || a1 % t == 0) printf("%d\n", a1 / t);
    else {
      int foo = Gcd(abs(a1), t);
      printf("%d/%d\n", a1 / foo, t / foo);
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
