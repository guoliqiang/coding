// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 17:15:54
// File  : code.cc
// Brief :

/*
Follow up for "Unique Paths":
Now consider if some obstacles are added to the grids. How many unique paths would there be?
An obstacle and empty space is marked as 1 and 0 respectively in the grid.

For example,
There is one obstacle in the middle of a 3x3 grid as illustrated below.

[
  [0,0,0],
  [0,1,0],
  [0,0,0]
]
The total number of unique paths is 2.
Note: m and n will be at most 100.

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 22/22 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 32 milli secs
 * Progress: 42/42 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

void DP(std::vector<std::vector<int> > & dp, int m, int n, std::vector<std::vector<int> > & grid) {
  for (int i = 0; i < n; i++) {
    if (grid[0][i]) dp[0][i] = 0;
    else {
      if (i != 0) dp[0][i] = dp[0][i - 1];
      else dp[0][i] = 1;
    }
  }
  for (int j = 0; j < m; j++) {
    if (grid[j][0]) dp[j][0] = 0;
    else {
      if (j != 0) dp[j][0] = dp[j - 1][0];
      else dp[j][0] = 1;
    }
  }
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (grid[i][j]) dp[i][j] = 0;
      else dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
    }
  }
}

int UniquePathDP(std::vector<std::vector<int> > & grid) {
  int m = grid.size();
  if (m == 0) return 0;
  int n = grid[0].size();
  if (n == 0) return 0;

  std::vector<std::vector<int> > & dp = grid;
  // std::vector<std::vector<int> >  dp(m, std::vector<int>(n, 0));
  DP(dp, m, n, grid);
  return dp[m - 1][n - 1];
}


}  // namespace algorithm

namespace twice {
int DP(std::vector<std::vector<int> > & grid) {
  int m = grid.size();
  int n = m > 0 ? grid[0].size() : 0;
  if (n == 0 || m == 0) return 0;
  
  std::vector<std::vector<int> > dp(2, std::vector<int>(n, 0));
  dp[0][0] = grid[0][0] == 1 ? 0 : 1;
  for (int i = 1; i < n; i++) {
      dp[0][i] = grid[0][i] == 1 ? 0 : dp[0][i - 1];
  }
  
  bool flag = false;
  for (int i = 1; i < m; i++) {
      for (int j = 0; j < n; j++) {
          if (j == 0) dp[!flag][j] = (grid[i][j] == 1) ? 0 : dp[flag][j];
          else dp[!flag][j] = (grid[i][j] == 1) ? 0 : dp[flag][j] + dp[!flag][j - 1];
      }
      flag = !flag;
  } 
  return dp[flag][n - 1];
}
}  // namespace twice

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<int> > grid(1, std::vector<int>(2, 0));
  grid[0][0] = 1;
  LOG(INFO) << UniquePathDP(grid);
  return 0;
}
