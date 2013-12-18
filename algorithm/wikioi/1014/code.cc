// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-16 16:47:16
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 33;
const int MAXW = 20008;
int N = 0;
int W = 0;
int dp[MAXW][MAXN];
int v[MAXN];

int Dp() {
  memset(dp, 0, sizeof(dp));
  for (int j = 1; j <= N; j++) {
    for (int i = 0; i <= W; i++) {
      dp[i][j] = dp[i][j - 1];
      if (i >= v[j])  dp[i][j] = std::max(dp[i][j], dp[i - v[j]][j - 1] + v[j]);
    }
  }
  return dp[W][N];
}

void Read() {
  while (scanf("%d", &W) != EOF) {
    getchar();
    scanf("%d", &N);
    getchar();
    memset(v, 0, sizeof(v));
    for (int i = 0; i < N; i++) {
      int t = 0;
      scanf("%d", &t);
      getchar();
      v[i + 1] = t;
    }
    int t = Dp();
    printf("%d\n", W - t);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
