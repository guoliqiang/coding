// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-02 11:22:23
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int Dp(std::vector<int> & v, int k) {
  v.insert(v.begin(), 0);
  k += 1;
  int n = v.size();
  std::vector<int> dp(n, 0);
  std::vector<int> pre(n, 0);
  int rs = 0;
  for (int i = 1; i < k; i++) {
    int max = INT_MIN;
    for (int j = k; j < n; j++) {
      dp[j] = std::max(dp[j - 1], pre[j - 1]) + v[j];
      pre[j - 1] = max;
      max = std::max(max, dp[j]);
      rs = std::max(rs, dp[j]);
    }
    pre[n - 1] = max;
  }
  return rs;
}

int MaxProfit(std::vector<int> & price, int n) {
  std::vector<int> v;
  for (int i = 1; i < price.size(); i++) {
    v.push_back(price[i] - price[i - 1]);
  }
  return Dp(v, n);
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
