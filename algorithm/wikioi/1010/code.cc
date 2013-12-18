// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-17 21:58:15
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 25;
int M;
int N;
int dp[MAXN][MAXN];

int Dp(int x, int y) {
  memset(dp, -1, sizeof(dp));
  dp[x][y] = 0;
  if (x - 2 >= 0) {
    if (y - 1 >= 0) dp[x - 2][y - 1] = 0;
    if (y + 1 < N) dp[x - 2][y + 1] = 0;
  }
  if (x + 2 <= M) {
    if (y - 1 >= 0) dp[x + 2][y - 1] = 0;
    if (y + 1 < N) dp[x + 2][y + 1] = 0;
  }
  if (y - 2 >= 0) {
    if (x + 1 < M) dp[x + 1][y - 2] = 0;
    if (x - 1 >= 0) dp[x - 1][y - 2] = 0;
  }
  if (y + 2 <= N) {
    if (x + 1 < M) dp[x + 1][y + 2] = 0;
    if (x - 1 >= 0) dp[x - 1][y + 2] = 0;
  }
  for (int i = 0; i <= M; i++) {
    for (int j = 0; j <= N; j++) {
      if (dp[i][j] == -1) {
        if (i == 0 && j == 0) dp[i][j] = 1;
        else if (i == 0) dp[i][j] = dp[i][j - 1];
        else if (j == 0) dp[i][j] = dp[i - 1][j];
        else dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
      }
    }
  }

  return dp[M][N];
}

void Read() {
  int x, y;
  while (scanf("%d%d%d%d", &M, &N, &x, &y) != EOF) {
    getchar();
    int t = Dp(x, y);
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
