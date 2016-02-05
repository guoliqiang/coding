// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 18:26:19
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

int MaxProfit(std::vector<int> &prices) {
  int rs = 0;
  for (int i = 1; i < prices.size() ; i++) {
     int delta = prices[i] - prices[i-1];
     if (delta > 0) rs += delta;
  }
  return rs;
}

int MaxProfitTLE(int k, std::vector<int>& prices) {
  if (k == 0 || prices.size() == 0) return 0;
  if (k > prices.size() / 2) return MaxProfit(prices);

  int m = k + 1;
  int n = prices.size();
  std::vector<std::vector<int> > dp(2, std::vector<int>(n, 0));

  int idx = 1;
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      int max = dp[idx][j - 1];
      for (int k = 0; k < j; k++) {
        if (k == 0) {
          max = std::max(max, prices[j] - prices[k]);
        } else {
          max = std::max(max, dp[(idx + 1) % 2][k - 1] + prices[j] - prices[k]);
        }
      }
      dp[idx][j] = max;
    }
    idx = (idx + 1) % 2;
  }
  return dp[(idx + 1) % 2][n - 1];
}

int MaxProfit(int k, std::vector<int>& prices) {
  if (k == 0 || prices.size() == 0) return 0;
  if (k > prices.size() / 2) return MaxProfit(prices);

  int m = k + 1;
  int n = prices.size();
  std::vector<std::vector<int> > dp(2, std::vector<int>(n, 0));

  int idx = 1;
  for (int i = 1; i < m; i++) {
    int tmp_max = -prices[0];
    for (int j = 1; j < n; j++) {
      int max = dp[idx][j - 1];
      max = std::max(max, prices[j] + tmp_max);
      tmp_max = std::max(tmp_max, dp[(idx + 1)%2][j - 1] - prices[j]);
      dp[idx][j] = max;
    }
    idx = (idx + 1) % 2;
  }
  return dp[(idx + 1) % 2][n - 1];
}


}  // namespace algorithm

using namespace algorithm;
namespace twice {
int MaxProfit(int k, std::vector<int>& prices) {
        if (k <= 0) return 0;

        std::vector<int> vec;
        for (int i = 1; i < prices.size(); i++) {
            vec.push_back(prices[i] - prices[i - 1]);
        }
        int n = vec.size();

        std::vector<std::vector<int> > dp(2, std::vector<int>(n, 0));
        std::vector<std::vector<int> > max(2, std::vector<int>(n, 0));
        int ans = 0;
        for (int i = 0; i < k; i++) {
            for (int j = i; j < n; j++) {
                if (i == 0) {
                    dp[i % 2][j] = (j - 1 >= 0) ? std::max(dp[i % 2][j - 1] + vec[j], vec[j]) : vec[j];
                } else {
                    dp[i % 2][j] = std::max(max[(i + 1) % 2][j - 1] + vec[j], dp[i % 2][j - 1] + vec[j]);
                    dp[i % 2][j] = std::max(dp[i % 2][j], dp[(i + 1) % 2][j]);
                }
                max[i % 2][j] = (j == 0) ? 0 : std::max(max[i % 2][j - 1], dp[i % 2][j - 1]);
                ans = std::max(ans, dp[i % 2][j]);
            }
        }
        return ans;
    }
}  // namespace twice

namespace clear {
using namespace std;

// 最大K段子段和问题的O(n*k) 写法
int MaxProfit(int k, vector<int>& prices) {
    if (k <= 0) return 0;
    std::vector<int> vec;
    for (int i = 1; i < prices.size(); i++) {
        vec.push_back(prices[i] - prices[i - 1]);
    }
    int n = vec.size();

    int ans = 0;
    std::vector<std::vector<int> > dp(k, std::vector<int>(n, 0));
    std::vector<std::vector<int> > max(k, std::vector<int>(n, 0));
    for (int i = 0; i < k; i++) {
        for (int j = i; j < n; j++) {
            if (i == 0) {
                dp[i][j] = (j - 1 >= 0) ? std::max(dp[i][j - 1] + vec[j], vec[j]) : vec[j];
            } else {
                dp[i][j] = std::max(max[i - 1][j - 1] + vec[j], dp[i][j - 1] + vec[j]);
            }
            max[i][j] = (j == 0) ? dp[i][j] : std::max(max[i][j - 1], dp[i][j]);
            ans = std::max(ans, dp[i][j]);
        }
    }
    return ans;
}
}  // namepsace clear
int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(4);
  vec.push_back(2);
  vec.push_back(5);
  vec.push_back(7);
  vec.push_back(2);
  vec.push_back(4);
  vec.push_back(9);
  vec.push_back(0);
  LOG(INFO) << clear::MaxProfit(1, vec);
  return 0;
}
