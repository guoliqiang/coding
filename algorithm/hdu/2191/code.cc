// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-16 17:45:25
// File  : code.cc
// Brief :

// Accepted 2191  0MS
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 105;
const int MAXW = 105;
int N;
int W;
int dp[MAXW][MAXN];
int w[MAXN];
int c[MAXN];
int p[MAXN];

int Dp() {
  memset(dp, 0, sizeof(dp));
  for (int i = 1; i <= N; i++) {
    for (int j = 0; j <= W; j++) {
      dp[i][j] = dp[i - 1][j];
      int t = std::min(c[i], j / p[i]);
      for (int k = 1; k <= t; k++) {
        dp[i][j] = std::max(dp[i][j], dp[i - 1][j - p[i] * k] + k * w[i]);
      }
    }
  }
  return dp[N][W];
}

void Read() {
  int cc = 0;
  while (scanf("%d", &cc) != EOF) {
    getchar();
    for (int i = 0; i < cc; i++) {
      memset(w, 0, sizeof(w));
      memset(c, 0, sizeof(c));
      memset(p, 0, sizeof(p));
      scanf("%d%d", &W, &N);
      getchar();
      for (int i = 0; i < N; i++) {
        int p1, w1, c1;
        scanf("%d%d%d", &p1, &w1, &c1);
        getchar();
        p[i + 1] = p1;
        w[i + 1] = w1;
        c[i + 1] = c1;
      }
      int t = Dp();
      printf("%d\n", t);
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
