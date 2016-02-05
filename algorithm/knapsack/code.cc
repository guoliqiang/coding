// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-16 16:39:22
// File  : code.cc
// Brief :

/*
完全背包问题(物品数量没有限制), 滚动数组从上到下更新，起始就是两种情况，不包含当前的物品，至少包含一个当前的物品.

当有背包数量的限制时(0/1背包), 滚动数组从下向上跟新，分成的情况是，不含当前物品，含有一个物品，含有两个物品，... 含有k个物品
*/
#include "base/public/common_ojhead.h"

// 0 - 1 背包每种物品只有一个
namespace algorithm {
const int MAXN = 100;
const int MAXW = 100;
int N = 0;
int W = 0;
int dp[MAXW][MAXN];
int dp2[MAXW];
int v[MAXN]; // v[0] 不存储物品
int c[MAXN];

int Dp() {
  memset(dp, 0, sizeof(dp));
  for (int j = 1; j <= N; j++) {
    for (int i = 0; i <= W; i++) {
      dp[i][j] = dp[i][j - 1];
      if (i >= v[j]) dp[i][j] = std::max(dp[i][j], dp[i - v[j]][j - 1] + c[i]);
    }
  }
  return dp[W][N];
}

// 滚动数组
int DPNB() {
  memset(dp2, 0, sizeof(dp2));  // 不一定装满
  for (int j = 1; j <= N; j++) {
    for (int i = W; i >= v[i]; i--) {  // 注意要从底向上
      dp2[i] = std::max(dp2[i], dp2[i - v[j]] + c[i]);
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
