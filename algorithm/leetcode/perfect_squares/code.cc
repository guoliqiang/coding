// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 15:13:22
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int NumSquares(int v) {
  int n = sqrt(v);
  std::vector<std::vector<int> > dp(v + 1, std::vector<int>(2, 0));
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= v; j++) {
      if (i == 1) dp[j][i % 2] = j;
      else {
        if (j >= i * i) {
          dp[j][i % 2] = std::min(dp[j][(i - 1) % 2], dp[j - i * i][i % 2] + 1);
        } else {
          dp[j][i % 2] = dp[j][(i - 1) % 2];
        }
      }
    }
  }
  return dp[v][n % 2];
}

}  // namespace algorithm

namespace NB {
int numSquares(int n) {
  if (n == 0) return 0;
  int v = sqrt(n);
  std::vector<int> dp(n + 1, INT_MAX);
  dp[0] = 0;

  for (int i = 1; i <= v; i++) {
    for (int j = i * i; j <= n; j++) {
      dp[j] = std::min(dp[j], dp[j - i * i] + 1);
    }
  }
  return dp[n];
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << NumSquares(13);
  LOG(INFO) << NumSquares(7);
  LOG(INFO) << NumSquares(12);
  LOG(INFO) << NumSquares(16);
  return 0;
}
