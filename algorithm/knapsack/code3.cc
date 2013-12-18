// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-16 17:34:49
// File  : code3.cc
// Brief :

#include "base/public/common_ojhead.h"

// 多重背包问题，背包的数量有上限限制
// hdu 2191
namespace algorithm {
const int MAXN = 100;
const int MAXW = 100;
int N;
int W;
int v[MAXN];
int c[MAXN];
int data[MAXN];
int dp[MAXW][MAXN];
int dp2[MAXW];

int Dp() {
  memset(dp, 0, sizeof(dp));
  for (int i = 1; i <= N; i++) {
    for (int j = 0; j <= W; j++) {
      dp[i][j] = dp[i - 1][j];
      int t = std::min(c[i], j / v[i]);
      for (int k = 1; k <= t; k++) {
        dp[i][j] = std::max(dp[i][j], dp[i][j - v[i] * k] + k * data[i]);
      }
    }
  }
  return dp[W][N];
}

// 滚动数组
int DPNB() {
  memset(dp2, 0, sizeof(dp2));
  for (int j = 1; j <= N; j++) {
    for (int i = W; i >= v[j]; i--) {  // 注意从底向上
      int t = std::min(c[i], i / v[j]);
      for (int k = 1; k <= t; k++) {
        dp2[i] = std::max(dp2[i], dp2[i - v[j] * k] + k * data[i]);
      }
    }
  }
  return dp2[W];
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
