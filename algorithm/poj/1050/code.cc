// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-23 21:46:45
// File  : code.cc
// Brief :

// Accepted 208K  16MS
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 105;
int N;
int data[MAXN][MAXN];
int v[MAXN];

int DpSub() {
  int rs = -INF;
  int t = 0;
  for (int i = 0; i < N; i++) {
    if (i == 0) t = v[i];
    else t = std::max(v[i], t + v[i]);
    rs = std::max(rs, t);
  }
  return rs;
}

int Dp() {
  int rs = -INF;
  for (int i = 0; i < N; i++) {
    memset(v, 0, sizeof(v));
    for (int j = i; j < N; j++) {
      for (int k = 0; k < N; k++) v[k] += data[j][k];
      rs = std::max(rs, DpSub());
    }
  }
  return rs;
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        scanf("%d", &(data[i][j]));
      }
    }
    int t = Dp();
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
