// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 17:35:38
// File  : code.cc
// Brief :

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

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << NumTrees(3);
  return 0;
}
