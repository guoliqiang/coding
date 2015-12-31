// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-28 11:27:52
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int CoinChange(std::vector<int>& coins, int amount) {
  int m = amount;
  int n = coins.size();
  if (m == 0 || n == 0) return 0;

  std::vector<std::vector<int> > dp(m + 1, std::vector<int>(2, 0));
  for (int i = 1; i <= m; i++) {
    if (i < coins[0]) {
      dp[i][0] = -1;
    } else {
      dp[i][0] = (dp[i - coins[0]][0] == -1) ? -1 : 1 + dp[i - coins[0]][0];
    }
  }

  for (int i = 1; i < n; i++) {
    for (int j = 1; j <= m; j++) {
      if (j < coins[i]) {
        dp[j][i % 2] = dp[j][(i - 1) % 2];
      } else {
        dp[j][i % 2] = (dp[j - coins[i]][i % 2] == -1) ? -1 : dp[j - coins[i]][i % 2] + 1;
        if (dp[j][(i - 1) % 2] != -1) {
          dp[j][i % 2] = (dp[j][i % 2] == -1) ? dp[j][(i - 1) % 2] :
                                      std::min(dp[j][i % 2], dp[j][(i - 1) % 2]);
        }
      }
    }
  }
  return dp[m][(n - 1) % 2];
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(5);
  LOG(INFO) << CoinChange(vec, 11);
  vec.clear();
  vec.push_back(2);
  LOG(INFO) << CoinChange(vec, 3);
  return 0;
}
