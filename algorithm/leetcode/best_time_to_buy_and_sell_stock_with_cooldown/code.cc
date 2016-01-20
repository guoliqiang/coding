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

namespace NB {

int MaxProfit(std::vector<int>& prices) {
  int n = prices.size();
  if (n == 0) return 0;

  std::vector<int> buy(n, 0);
  std::vector<int> sell(n, 0);
  std::vector<int> rest(n, 0);

  for (int i = 0; i < n; i++) {
    if (i == 0) {
      buy[i] = -prices[i];
    } else {
      buy[i] = std::max(buy[i - 1], rest[i - 1] - prices[i]);
      sell[i] = std::max(sell[i - 1], buy[i - 1] + prices[i]);
      rest[i] = std::max(rest[i - 1], std::max(sell[i - 1], buy[i - 1]));
    }
  }
  return std::max(0, sell[n - 1]);
}

}  // namespace NB

namespace clear {
// 三个状态，如果当前不操作，是前面最后一次状态的延续，这样很清晰
int maxProfit(std::vector<int>& prices) {
        int n = prices.size();
        if (n == 0) return 0;

        std::vector<int> buy(n, 0);
        std::vector<int> seller(n, 0);
        std::vector<int> cool(n, 0);

       for (int i = 0; i < n; i++) {
            if (i == 0) {
                buy[i] = - prices[i];
            } else {
                // 最后一个是卖的，则当前一定是卖, cool 只能发生在seller之后
                seller[i] = buy[i - 1] + prices[i];
                buy[i] = std::max(buy[i - 1], cool[i - 1] - prices[i]);
                cool[i] = std::max(cool[i - 1], seller[i - 1]);
            }
        }
        return std::max(seller[n - 1], cool[n - 1]);
    }
}  // namespace clear

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
