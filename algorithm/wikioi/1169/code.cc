// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-17 23:11:39
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 10;   // 提交是修改为52
int M;
int N;
int dp[MAXN][MAXN][MAXN][MAXN];
int data[MAXN][MAXN];
int Dp() {
  memset(dp, 0, sizeof(dp));
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < M; k++) {
        for (int l = 0; l < N; l++) {
          int t = data[i][j] + data[k][l];
          if (i - 1 >= 0 && k - 1 >= 0 && (i != k || j != l || (i - 1 == 0 && j == 0 && k - 1 == 0 && l == 0)))
            dp[i][j][k][l] = std::max(dp[i][j][k][l], dp[i - 1][j][k - 1][l] + t);
          if (i - 1 >= 0 && l - 1 >= 0 && (i - 1 != k || j != l - 1 || (i - 1 == 0 && j == 0 && k == 0 && l - 1 == 0)))
            dp[i][j][k][l] = std::max(dp[i][j][k][l], dp[i - 1][j][k][l - 1] + t);
          if (j - 1 >= 0 && k - 1 >= 0 && (i != k - 1 || j - 1 != l || (i == 0 && j - 1 == 0 && k - 1 == 0 && l == 0)))
            dp[i][j][k][l] = std::max(dp[i][j][k][l], dp[i][j - 1][k - 1][l] + t);
          if (j - 1 >= 0 && l - 1 >= 0 && (i != k || j != l || (i == 0 && j - 1 == 0 && k == 0 && l - 1 == 0)))
            dp[i][j][k][l] = std::max(dp[i][j][k][l], dp[i][j - 1][k][l - 1] + t);
        }
      }
    }
  }
  return dp[M - 1][N - 1][M - 1][N - 1];
}

void Read() {
  while (scanf("%d%d", &M, &N) != EOF) {
    getchar();
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < N; j++) {
        int t = 0;
        scanf("%d", &t);
        data[i][j] = t;
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

