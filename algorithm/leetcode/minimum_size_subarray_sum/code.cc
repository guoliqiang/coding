// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 15:25:39
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int MinSubArrayLen(int s, std::vector<int>& nums) {
  int n = nums.size();
  if (n == 0) return 0;

  std::vector<int> dp(n, 0);
  if (nums[0] >= s) dp[0] = 1;
  for (int i = 1; i < n; i++) {
    if (dp[i - 1] > 0) dp[i] = dp[i - 1];
    int sum = 0;
    int k = i;
    while (sum < s && k >= 0) {
      sum += nums[k];
      k--;
    }
    if (sum >= s) {
      dp[i] = dp[i] > 0 ? std::min(dp[i], i - k) : i - k;
    }
  }
  return dp[n - 1];
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(4);
  vec.push_back(3);
  LOG(INFO) << MinSubArrayLen(100, vec);
  return 0;
}
