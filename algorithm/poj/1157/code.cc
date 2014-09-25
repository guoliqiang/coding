// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-27 21:12:14
// File  : code.cc
// Brief :

// Accepted 228K  16MS

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 110;
int N;
int M;
int data[MAXN][MAXN];
int dp[MAXN][MAXN];

int Dp() {
  memset(dp, 0, sizeof(dp));
  for (int i = 0; i < N; i++) {
    if (i == 0) dp[0][i] = data[0][i];
    else dp[0][i] = std::max(dp[0][i - 1], data[0][i]);
  }
  for (int i = 1; i < M; i++) {
    for (int j = i; j < N; j++) {
      if (j == i) {
        dp[i][j] = dp[i - 1][j - 1] + data[i][j];
      } else {
        int t = dp[i - 1][j - 1] + data[i][j];
        dp[i][j] = std::max(dp[i][j - 1], t);
      }
    }
  }
  return dp[M - 1][N - 1];
}

void Read() {
  memset(data, 0, sizeof(data));
  scanf("%d%d", &M, &N);
  getchar();
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      scanf("%d", &(data[i][j]));
    }
    getchar();
  }
  int t = Dp();
  printf("%d\n", t);
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
