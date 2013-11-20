// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-03 06:09:00
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

/*
 * f(n, m) 表示n个数据，逆序数为m时的不同排列的个数。
 * 则 f(n + 1, m) = f(n, m) + f(n, m - 1) + f(n, m -2) + ... f(n, m - n)  // 根据第n+1个数产生的逆序数分类
 *    
 *    f(n + 1, m) = f(n, m) + f(n + 1, m - 1) - f(n, m - n - 1)
 *
 * TODO poj3761
 * */

namespace algorithm {
const int MAX = 1000;
int N = 0;
int dp[MAX][MAX] = {{0}};

void DP() {
  memset(dp, 0, sizeof(dp));
  dp[1][0] = dp[2][0] = dp[2][1] = 1;
  for (int i = 3; i <= N; i++) {
    dp[i][0] = 1;
    int m = i * (i - 1) / 2;
    for (int j = 1; j <= m; j++) {
      dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
      if (j - i >= 0) dp[i][j] -= dp[i - 1][j - i];
    }
  }
}

int Get(int i, int j) {
  return dp[i][j];
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  N = 5;
  DP();
  LOG(INFO) << Get(3, 2);
  LOG(INFO) << Get(4, 2);
  return 0;
}
