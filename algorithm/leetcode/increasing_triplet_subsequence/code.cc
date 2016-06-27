// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-03-20 23:10:52
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
bool increasingTriplet(std::vector<int>& nums) {
  int n = nums.size();
  std::vector<int> dp(n, 0);
  for (int i = 0; i < n; i++) {
    if (i == 0) dp[i] = nums[i];
    else dp[i] = std::min(dp[i - 1], nums[i]);
  }

  int max = 0;
  for (int i = n - 1; i > 0; i--) {
    if (i == n - 1) max = nums[i];
    else {
      if (dp[i - 1] < nums[i] && max > nums[i]) return true;
      max = std::max(max, nums[i]);
    }
  }
  return false;
}
}  // namespace algorithm

int main(int argc, char** argv) {
  return 0;
}
