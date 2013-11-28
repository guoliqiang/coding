// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 19:53:03
// File  : code.cc
// Brief :

/*
Given a m x n grid filled with non-negative numbers, find a path from top left to bottom right which minimizes the sum of all numbers along its path.
Note: You can only move either down or right at any point in time.

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 10/10 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 76 milli secs
 * Progress: 61/61 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {
void DP(std::vector<std::vector<int> > & dp, std::vector<std::vector<int> > & grid) {
  int m = grid.size();
  int n = grid[0].size();
  dp[0][0] = grid[0][0];

  for (int i = 1; i < m; i++) {
    dp[i][0] = dp[i - 1][0] + grid[i][0];
  }

  for (int i = 1; i < n; i++) {
    dp[0][i] = dp[0][i - 1] + grid[0][i];
  }

  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      dp[i][j] = std::min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
    }
  }
}

int MinPathSum(std::vector<std::vector<int> > & grid) {
  if (grid.size() == 0) return 0;
  if (grid[0].size() == 0) return 0;
  std::vector<std::vector<int> > dp(grid.size(), std::vector<int>(grid[0].size(), 0));
  DP(dp, grid);
  return dp[grid.size() - 1][grid[0].size() - 1];
}

}  // namespace algorithm

namespace twice {
int DP(std::vector<std::vector<int> > & grid) {
  int m = grid.size();
  int n = m > 0 ? grid[0].size() : 0;
  if (n == 0 || m == 0) return 0;
  std::vector<std::vector<int> > dp(2, std::vector<int>(n, 0));
  dp[0][0] = grid[0][0];
  for (int i = 1; i < n; i++) dp[0][i] = dp[0][i - 1] + grid[0][i];
  bool flag = false;
  for (int i = 1; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (j == 0) dp[!flag][j] = dp[flag][j];
      else dp[!flag][j] = std::min(dp[flag][j], dp[!flag][j - 1]);
      dp[!flag][j] += grid[i][j];
    }
    flag = !flag;
  }
  return dp[flag][n - 1];
}

}  // namespace twice
using namespace algorithm;


int main(int argc, char** argv) {
  return 0;
}
