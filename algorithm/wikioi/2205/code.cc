// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-21 21:28:52
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1005;
int N;
const int MAXM = 2012;
int v[MAXN];
int dp[MAXN][MAXM];

int Dp() {
  memset(dp, 0, sizeof(dp));
  for (int i = 1; i < N; i++) {
    for (int j = i - 1; j >= 0; j--) {
      int t = 1010 + v[i] - v[j];
      dp[i][t] += (dp[j][t] + 1);
      dp[i][t] %= 9901;
    }
  }
  int rs = N;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < MAXM; j++) {
      rs = (rs + dp[i][j]) % 9901;
    }
  }
  return rs;
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    for (int i = 0; i < N; i++) {
      int t = 0;
      scanf("%d", &t);
      v[i] = t;
    }
    getchar();
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
