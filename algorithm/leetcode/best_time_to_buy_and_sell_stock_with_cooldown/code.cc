// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-24 20:02:35
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int MaxProfit(std::vector<int>& prices) {
  int n = prices.size();
  if (n == 0) return 0;
  int ans = 0;

  std::vector<int> dp(n, 0);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      if (prices[j] >= prices[i]) {
        dp[i] = std::max(dp[i], dp[j]);
      } else {
        dp[i] = std::max(dp[i], prices[i] - prices[j] +
                                (j - 2 < 0 ? 0 : dp[j - 2]));
      }
    }
    ans = std::max(ans, dp[i]);
  }
  LOG(INFO) << JoinVector(dp);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(0);
  vec.push_back(2);
  LOG(INFO) << MaxProfit(vec);
  return 0;
}
