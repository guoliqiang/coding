// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 18:07:24
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int Rob(std::vector<int>& nums) {
  int n = nums.size();
  if (n == 0) return 0;

  std::vector<int> dp(n, 0);
  std::vector<int> dp2(n, 0);
  dp[0] = 0;
  dp2[0] = nums[0];

  for (int i = 1; i < n; i++) {
    dp[i] = dp[i - 1];
    int tmp = i - 2 >= 0 ? dp[i - 2] : 0;
    if (tmp + nums[i] > dp[i]) dp[i] = tmp + nums[i];

    if (i == n - 1) {
      dp2[i] = dp2[i - 1];
    } else {
      dp2[i] = dp2[i - 1];
      tmp = i - 2 >= 0 ? dp2[i - 2] : 0;
      if (tmp + nums[i] > dp2[i]) dp2[i] = tmp + nums[i];
    }
  }
  return std::max(dp[n - 1], dp2[n - 1]);
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
