// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-16 05:10:15
// File  : code.cc
// Brief :

/*
 * Accepted 132K  32MS
 * 居然不TLE数据好弱
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {

int Last(int n, int m) {
  if (n == 1) return 0;
  else {
    int t = (Last(n - 1, m) + m) % n;
    return t;
  }
}

int LastWrapper(int n, int m) {
  return Last(n, m) + 1;
}

int MinM(int n) {
  int m = 1;
  while (LastWrapper(n, m) != 1) {
    m++;
  }
  return m;
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF && n) {
    printf("%d\n", MinM(n - 1));
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
