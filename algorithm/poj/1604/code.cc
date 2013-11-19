// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-18 18:09:52
// File  : code.cc
// Brief :

/*
 *  Accepted  132K  0MS
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
int data[] = {1, 1, 2, 6, 4, 2, 2, 4, 2, 8};  // 0 1 2 3 4 5 6 7 8 9

int LastNum(int k) {
  if (k < 10) return data[k];
  int t1 = k / 10;
  int t2 = k % 10;
  if (t1 % 2 == 1) {
    return (4 * data[t2] * LastNum(k / 5)) % 10;
  } else {
    return (6 * data[t2] * LastNum(k / 5)) % 10;
  }
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF) {
    int t = LastNum(n);
    printf("%5d -> %d\n", n, t);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
