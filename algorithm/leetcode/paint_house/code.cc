// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-10 09:50:06
// File  : code.cc
// Brief :

/*
There are a row of n houses, each house can be painted with one of the three colors: red, blue or green. The cost of painting each house with a certain color is different. You have to paint all the houses such that no two adjacent houses have the same color.

The cost of painting each house with a certain color is represented by a n x 3 cost matrix. For example, costs[0][0] is the cost of painting house 0 with color red;costs[1][2] is the cost of painting house 1 with color green, and so on... Find the minimum cost to paint all houses.

Note:
All costs are positive integers.
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
int MinCost(std::vector<std::vector<int> >& costs) {
  int n = costs.size();
  std::vector<std::vector<int> > dp(3, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 3; j++) {
      if (i == 0) dp[i][j] = costs[i][j];
      else {
        dp[i][j] = std::min(dp[i - 1][(j + 1) % 3], dp[i - 1][(j + 2) % 3]) + costs[i][j];
      }
    }
  }
  return std::min(dp[0][n - 1], std::min(dp[1][n - 1], dp[2][n - 1]));
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
