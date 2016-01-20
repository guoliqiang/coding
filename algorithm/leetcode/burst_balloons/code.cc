// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-29 14:46:09
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int MaxCoins(std::vector<int>& nums) {
  int n = nums.size();
  nums.insert(nums.begin(), 1);
  nums.push_back(1);

  std::vector<std::vector<int> > dp(n + 2, std::vector<int>(n + 2, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 1; j + i <= n; j++) {
      if (i == 0) {
        dp[j][j + i] = nums[j - 1] * nums[j] * nums[j + 1];
      } else {
        for (int k = j; k <= j + i; k++) {
          int cur = nums[k] * nums[j - 1] * nums[i + j + 1] + dp[j][k - 1] + dp[k + 1][i + j];
          dp[j][j + i] = std::max(dp[j][j + i], cur);
        }
      }
    }
  }
  return dp[1][n];
}
}  // namespace algorithm

using namespace algorithm;

namespace twice {
int maxCoins(vector<int>& nums) {
  int n = nums.size();
  if (n == 0) return 0;

  nums.insert(nums.begin(), 1);
  nums.push_back(1);

  std::vector<std::vector<int> > dp(n + 2, std::vector<int>(n + 2, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 1; j + i <= n; j++) {
      for (int k = j; k <= j + i; k++) {
        int cur = nums[k] * nums[j - 1] * nums[j + i + 1] + dp[j][k - 1] + dp[k + 1][j + i];
        dp[j][j + i] = std::max(dp[j][j + i], cur);
      }
    }
  }
  return dp[1][n];
}
}  // namespace twice

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(3);
  vec.push_back(1);
  vec.push_back(5);
  vec.push_back(8);
  LOG(INFO) << MaxCoins(vec);
  return 0;
}
