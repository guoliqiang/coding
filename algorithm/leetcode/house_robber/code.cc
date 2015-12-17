// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-17 22:51:45
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

int Rob(std::vector<int>& nums) {
  int n = nums.size();
  if (n == 0) return 0;

  std::vector<int> dp(n, 0);
  for (int i = 0; i < n; i++) {
    if (i == 0) dp[0] = nums[0];
    else if (i == 1) dp[1] = std::max(nums[0], nums[1]);
    else {
      dp[i] = std::max(dp[i - 1], dp[i - 2] + nums[i]);
    }
  }
  return dp[n - 1];
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}


