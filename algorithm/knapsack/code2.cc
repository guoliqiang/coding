// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-16 17:12:17
// File  : code2.cc
// Brief :

// 完全背包问题，不限制物品的数量
// poj 1384

/*
dp[i][j]: 表示前j个物品中，总重量不会超过i的组合的最大价值。

变形(如poj1384)：
dp[i][j]:表示前j个物品中，总重量恰好为i的组合的最大价值。

tyvj1214
*/
#include "base/public/common_ojhead.h"
namespace algorithm {
const int MAXN = 100;
const int MAXW = 100;
int dp[MAXW][MAXN];
int dp2[MAXW];
int v[MAXN];
int data[MAXN];
int N;
int W;

int Dp() {
  memset(dp, 0, sizeof(dp));
  for (int j = 1; j <= N; j++) {
    for (int i = 0; i <= W; i++) {  // 使用二维的dp, i要从０开始循环
      dp[i][j] = dp[i][j - 1];
      if (i >= v[j])
        dp[i][j] = std::max(dp[i][j], dp[i - v[j]][j] + data[j]);  //此处和0，1背包不同
    }
  }
  return dp[W][N];
}

// 滚动数组
// NB
int DpNB() {
  memset(dp2, 0, sizeof(dp2));  // 背包不一定要满
  for (int j = 1; j <= N; j++) {
    for (int i = v[j]; i <= W; i++) {  // 注意要从上到下
      dp2[i] = std::max(dp2[i], dp2[i - v[j]] + data[j]);
    }
  }
  return dp2[W];
}

// 要求背包装满
int DpNB2() {
  memset(dp2, 0, sizeof(dp2));
  dp2[0] = 0;
  for (int i = 1; i <= W; i++) dp2[i] = -INF;
  for (int j = 1; j <= N; j++) {
    for (int i = v[j]; i <= W; i++) {
      dp2[i] = std::max(dp2[i], dp2[i - v[j]] + data[j]);
    }
  }
  return dp2[W];  // 当dp2[W] < 0时背包不能被装满
}

// 要求背包装满
int DpNB3() {
  memset(dp2, 0, sizeof(dp2));
  dp2[0] = 0;
  for (int i = 1; i <= W; i++) dp2[i] = INF;
  for (int j = 1; j <= N; j++) {
    for (int i = v[j]; i <= W; i++) {
      dp2[i] = std::min(dp2[i], dp2[i - v[j]] + data[j]);
    }
  }
  return dp2[W];  // 当dp2[W] == INF 时背包不能被装满
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
