// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 17:35:38
// File  : code.cc
// Brief :

/*
Given n, how many structurally unique BST's (binary search trees) that store values 1...n?

For example,
Given n = 3, there are a total of 5 unique BST's.

   1         3     3      2      1
    \       /     /      / \      \
     3     2     1      1   3      2
    /     /       \                 \
   2     1         2                 3

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 3/3 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 0 milli secs
 * Progress: 14/14 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

// 树形dp标准写法
// 二维树形dp
void DP(std::vector<std::vector<int> > & dp) {
  int  n = dp.size();
  for (int i = 0; i < n; i++) {
    dp[i][i] = 1;
  }
  for (int i = 1; i < n; i++) {
    for (int j = 0; j < n - i; j++) {
      int sum = 0;
      for (int k = j; k <= j + i; k++) {
        if (k == j) sum += dp[k + 1][j + i];
        else if (k == j + i)  sum += dp[j][j + i - 1];
        else sum += dp[j][k - 1] * dp[k + 1][j + i];
      }
      dp[j][j + i] = sum;
    }
  }
  // LOG(INFO) << JoinMatrix(&dp);
}

int NumTrees(int n) {
  if (n <= 0) return 0;
  std::vector<std::vector<int> > dp(n, std::vector<int>(n, 0));
  DP(dp);
  return dp[0][n - 1];
}

}  // namespace algorithm

namespace twice {
int DP(int n) {
  std::vector<std::vector<int> > dp(n, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) dp[i][0] = 1;
  for (int  k = 1; k < n; k++) {
    for (int j = 0; j < n - k; j++) {
      int sum = 0;
      for (int i = 0; i <= k; i++) {
        if (i == 0) sum += dp[j + 1][k - 1];
        else if (i == k) sum +=  dp[j][k - 1];
        else sum += dp[j][i - 1] * dp[j + i + 1][k - i - 1];
      }
      dp[j][k] = sum;
    }
  }
  return dp[0][n - 1];
}
}   // namespace twice

namespace third {
long long Factorial(long long n) {
  if (n == 1 || n == 0) return 1;
  return n * Factorial(n - 1);
}

int C(int n, int m) {
  long long rs = 1;
  for (int i = 0; i < m; i++) rs *= (n - i);
  for (int i = 2; i <= m; i++) rs /= i;
  return rs;
  // n = 11 会越界
  // return Factorial(n) / (Factorial(m) * Factorial(n - m));
}

int C2(int n, int m) {
  m = std::min(m, n - m);
  int rs = 1;
  int j = 2;
  for (int i = 0; i < m; i++) {
    rs *= n - i;
    while (j <= m && rs % j == 0) {
        rs /= j;
        j++;
    }
  }
  // 此时一定有 j = m + 1
  return rs;
}

int NumTrees(int n) {
  int t = C2(2 * n, n);
  return t / (n + 1);
}
}  // namespace third

namespace NB {
// explain this equation
// http://oj.leetcode.com/discuss/620/attention-adding-lines-help-reducing-running-time-from-1140ms
int NumTrees(int n) {
  LOG(INFO) << "n:" << n;
  if (n == 0 || n == 1 || n == 2) return n;
  int rs = 2;
  for (int i = 3; i <= n; i++) {
    LOG(INFO) << "rs:" << rs;
    LOG(INFO) << "rs * 2 * " << 2 * i - 1;
    LOG(INFO) << "rs / " << i + 1;
    rs = rs * 2 * (2 * i - 1) / (i + 1);
  }
  return rs;
}

}  // namespace NB

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << NumTrees(7);
  LOG(INFO) << third::NumTrees(7);
  LOG(INFO) << twice::DP(7);
  LOG(INFO) << NB::NumTrees(7);
  return 0;
}
