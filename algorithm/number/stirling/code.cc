// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-28 19:56:52
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

// 第二类stirling数
// 把从1到p标号的p个球放到k个无区别的盒子里，要求每个盒子里至少有一个小球，
// 问不同的放法？
// S(p, k) = S(p - 1, k - 1) + k *S(p - 1, k)
int Stirling2(int n, int k) {
  if (k > n) return 0;
  std::vector<std::vector<int> > dp(n + 1, std::vector<int>(k + 1, 0));
  dp[0][0] = 0;
  for (int i = 1; i<= n; i++) {
    for (int j = 0; j <= std::min(i, k); j++) {
      if (j == 0) dp[i][j] = 0;
      else if (j == i || j == 1) dp[i][j] = 1;
      else {
        dp[i][j] = j * dp[i - 1][j] + dp[i - 1][j - 1];
      }
      // LOG(INFO) << "dp[" << i << "][" << j << "]:" << dp[i][j];
    }
  }
  return dp[n][k];
}

void Stirling2(int n, int k, std::vector<std::vector<int> > & dp) {
  dp.resize(n + 1, std::vector<int>(k + 1, 0));
  dp[0][0] = 0;
  for (int i = 1; i<= n; i++) {
    for (int j = 0; j <= std::min(i, k); j++) {
      if (j == 0) dp[i][j] = 0;
      else if (j == i || j == 1) dp[i][j] = 1;
      else {
        dp[i][j] = j * dp[i - 1][j] + dp[i - 1][j - 1];
      }
      // LOG(INFO) << "dp[" << i << "][" << j << "]:" << dp[i][j];
    }
  }
}

// poj1671
// bell数的计算基于第二类stirling数
// 基数为n的集合的划分方法的数目.
// B(n) = sum(S(n, k))  (1<=k<=n)
int Bell(int n) {
  std::vector<std::vector<int> > dp;
  Stirling2(n, n, dp);
  int rs = 0;
  for (int i = 1; i <= n; i++) {
    rs += dp[n][i];
    // LOG(INFO) << "dp[" << n << "][" << i << "]:" << dp[n][i];
  }
  return rs;
}

// 第一类stirling数
// 将p个物体排成k个非空循环排列的方法数？
// S(p, k) = S(p - 1, k - 1) + (p - 1) * S(p - 1, k)
int Stirling(int n, int k) {
  if (k > n || n == 0) return 0;
  std::vector<std::vector<int> > dp(n + 1, std::vector<int>(k + 1, 0));
  dp[1][1] = 1;
  dp[1][0] = 0;
  for (int i = 2; i <= n; i++) {
    for (int j = 0; j <= std::min(i, k); j++) {
      if (j == 0) dp[i][j] = 0;
      else {
        dp[i][j] = dp[i - 1][j - 1] + (i - 1) * dp[i - 1][j];
      }
    }
  }
  return dp[n][k];
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Stirling2(4, 2);
  LOG(INFO) << Bell(3);
  LOG(INFO) << Stirling(4, 2);
  return 0;
}
