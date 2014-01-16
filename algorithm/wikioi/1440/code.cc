// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-14 22:30:06
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int data[] = {1, 1, 2, 6, 4, 2, 2, 4, 2, 8};

int Last(int k) {
  if (k < 10) return data[k];
  int rs = 0;
  if ((k / 10) % 2 == 1) rs = 4;
  else rs = 6;
  rs = rs * data[k % 10] * Last(k / 5);
  return rs % 10;
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF) {
    getchar();
    int t = Last(n);
    printf("%d\n", t);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
