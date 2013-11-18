// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-18 04:34:40
// File  : code.cc
// Brief :

// 取石子游戏，见number/
// Accepted  144K   0MS
#include "base/public/common_ojhead.h"

namespace algorithm {

double p = (1 + sqrt(5.0)) / 2;

bool Win(int a, int b) {
  if (a > b) std::swap(a, b);
  int t = (b - a) * p;
  return a == t ? false : true;
}

void Read() {
  int a, b;
  while (scanf("%d%d", &a, &b) != EOF) {
    getchar();
    if (Win(a, b)) printf("1\n");
    else printf("0\n");
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
