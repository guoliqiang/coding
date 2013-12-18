// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-17 21:41:37
// File  : code.cc
// Brief :

// Accepted 216K  32MS

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 105;
int N;
int dp[MAXN][MAXN];
int Dp() {
  for (int i = 1; i < N; i++) {
    for (int j = 0; j < i + 1; j++) {
      if (j == 0)  dp[i][j] += dp[i - 1][j];
      else if (j == i) dp[i][j] += dp[i - 1][j - 1];
      else dp[i][j] += std::max(dp[i - 1][j - 1], dp[i - 1][j]);
    }
  }
  int max = -INF;
  for (int i = 0; i < N; i++) {
    max = std::max(max, dp[N - 1][i]);
  }
  return max;
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    memset(dp, 0, sizeof(dp));
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < i + 1; j++) {
        int t = 0;
        scanf("%d", &t);
        dp[i][j] = t;
      }
      getchar();
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
