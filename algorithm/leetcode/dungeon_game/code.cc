// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 12:08:49
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

int Calculate(std::vector<std::vector<int> > & data, int m, int n, int val) {
  if (data[m][n] < 0) val += -data[m][n];
  else val = std::max(0, val - data[m][n]);

  if (m == 0 && n == 0) {
    return val;
  } else if (m == 0) {
    return Calculate(data, m, n - 1, val);
  } else if (n == 0) {
    return Calculate(data, m - 1, n, val);
  } else {
    return std::min(Calculate(data, m, n - 1, val),
                    Calculate(data, m - 1, n, val));
  }
}

}  // namespace algorithm

namespace DP {

int Calculate(std::vector<std::vector<int> > & data) {
  int m = data.size();
  int n = data[0].size();

  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  dp[m - 1][n - 1] = std::max(1, 1 - data[m - 1][n - 1]);
  for (int i = m - 2; i >= 0; i--) {
    dp[i][n - 1] = std::max(1, dp[i + 1][n - 1] - data[i][n - 1]);
  }
  for (int i = n - 2; i >=0; i--) {
    dp[m - 1][i] = std::max(1, dp[m - 1][i + 1] - data[m - 1][i]);
  }
  for (int i = m - 2; i >= 0; i--) {
    for (int j = n - 2; j>=0; j--) {
      int cur1 = std::max(1, dp[i + 1][j] - data[i][j]);
      int cur2 = std::max(1, dp[i][j + 1] - data[i][j]);
      dp[i][j] = std::min(cur1, cur2);
    }
  }
  return dp[0][0];
}

}  // namespace DP

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<int> > data(3, std::vector<int>(3, 0));
  data[0][0] = -2; data[0][1] = -3; data[0][2] = 3;
  data[1][0] = -5; data[1][1] = -10; data[1][2] = 1;
  data[2][0] = 10; data[2][1] = 30; data[2][2] = -5;
  LOG(INFO) << Calculate(data, 2, 2, 1);
  LOG(INFO) << DP::Calculate(data);
  return 0;
}
