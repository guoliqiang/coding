// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-10 23:10:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 300008;
int N;

struct Node {
  int index;
  int val;
  int dp;
} data[MAXN];

bool CmpV(const Node & x, const Node & y) {
  return x.val < y.val;
}

bool CmpI(const Node & x, const Node & y) {
  return x.index < y.index;
}

void Dp() {
  std::sort(data, data + N, CmpV);
  data[0].dp = data[0].val;
  for (int i = 1; i < N; i++) {
    data[i].dp = std::max(data[i - 1].dp + 1, data[i].val);
  }

  std::sort(data, data + N, CmpI);
  for (int i = 0; i < N; i++) printf("%d ", data[i].dp);
  printf("\n");
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    for (int i = 0; i < N; i++) {
      int t = 0;
      scanf("%d", &t);
      data[i].index = i;
      data[i].val = t;
      data[i].dp = 0;
    }
    getchar();
    Dp();
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
