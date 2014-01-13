// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-10 21:04:30
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int Solve(int a, int b) {
  int rs = 0;
  while (a) {
    int t = a / b;
    if (t == 0) {
      rs += a;
      a = 0;
    } else {
      rs += b * t;
      a = a - b * t + t;
    }
  }
  return rs;
}

void Read() {
  int a, b;
  while (scanf("%d%d", &a, &b) != EOF) {
    getchar();
    int t = Solve(a, b);
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
