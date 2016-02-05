// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-10 10:36:10
// File  : code.cc
// Brief :

/*
There are a row of n houses, each house can be painted with one of the k colors. The cost of painting each house with a certain color is different. You have to paint all the houses such that no two adjacent houses have the same color.

The cost of painting each house with a certain color is represented by a n x k cost matrix. For example, costs[0][0] is the cost of painting house 0 with color 0; costs[1][2] is the cost of painting house 1 with color 2, and so on... Find the minimum cost to paint all houses.

Note:
All costs are positive integers.

Follow up:
Could you solve it in O(nk) runtime?
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
int MinCostII(std::vector<std::vector<int> > & costs) {
  int m = costs.size();
  if (m == 0) return 0;
  int n = costs[0].size();

  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  int first_min[2] = {0, 0};
  int first_idx[2] = {-1, -1};
  int second_min[2] = {0, 0};
  for (int i = 0; i < m; i++) {
    first_min[i % 2] = INT_MAX;
    second_min[i % 2] = INT_MAX;
    for (int j = 0; j < n; j++) {
      if (i == 0) {
        dp[i][j] = costs[i][j];
      } else {
        if (j == first_idx[(i - 1) % 2]) {
          dp[i][j] = second_min[(i - 1) % 2] + costs[i][j];
        } else {
          dp[i][j] = first_min[(i - 1) % 2] + costs[i][j];
        }
      }
      if (dp[i][j] < first_min[i % 2]) {
        first_min[i % 2] = dp[i][j];
        first_idx[i % 2] = j;
      } else if (dp[i][j] < second_min[i % 2]) {
        second_min[i % 2] = dp[i][j];
      }
    }
  }
  int ans = INT_MAX;
  for (int i = 0; i < m; i++) ans = std::min(ans, dp[i][n - 1]);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

// 这个最清晰
namespace NB {
int MinCostII(std::vector<std::vector<int> > & costs) {
  int m = costs.size();
  if (m == 0) return 0;
  int n = costs[0].size();

  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) dp[0][i] = costs[0][i];

  for (int i = 1; i < m; i++) {
    // 快速求去掉某个元素后的最小值
    std::vector<int> min1(m, INT_MAX);
    std::vector<int> min2(m, INT_MAX);
    for (int j = 1; j < n; j++) {
      min1[j] = std::min(min1[j - 1], dp[i - 1][j - 1]);
    }
    for (int j = n - 2; j >= 0; j--) {
      min2[j] = std::min(min2[j + 1], dp[i - 1][j + 1]);
    }
    for (int j = 0; j < n; j++) {
      dp[i][j] = costs[i][j] + std::min(min1[j], min2[j]);
    }
  }
  int ans = INT_MAX;
  for (int i = 0; i < m; i++) ans = std::min(ans, dp[i][n - 1]);
  return ans;
}
}  // namespace NB

namespace NB2 {
int MinCostII(std::vector<std::vector<int> > & costs) {
  int m = costs.size();
  if (m == 0) return 0;
  int n = costs[0].size();

  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  int last_min1 = -1;
  int last_min2 = -1;
  int min1 = -1;
  int min2 = -1;
  for (int i = 0; i < n; i++) {
    dp[0][i] = costs[0][i];
    if (last_min1 == -1 || dp[0][last_min1] > dp[0][i]) {
      last_min2 = last_min1;
      last_min1 = i;
    } else if (last_min2 == -1 || dp[0][last_min2] > dp[0][i]) {
      last_min2 = i;
    }
  }

  for (int i = 1; i < m; i++) {
    min1 = -1;
    min2 = -1;
    for (int j = 0; j < n; j++) {
      if (j == last_min1) dp[i][j] = dp[i - 1][last_min2] + costs[i][j];
      else dp[i][j] = dp[i - 1][last_min1] + costs[i][j];

      if (min1 == -1 || dp[i][min1] > dp[i][j]) {
        min2 = min1;
        min1 = j;
      } else if (min2 == -1 || dp[i][min2] > dp[i][j]) {
        min2 = j;
      }
    }
    last_min1 = min1;
    last_min2 = min2;
  }

  int ans = INT_MAX;
  for (int i = 0; i < m; i++) ans = std::min(ans, dp[i][n - 1]);
  return ans;
}
}  // namespace NB2

int main(int argc, char** argv) {
  return 0;
}
