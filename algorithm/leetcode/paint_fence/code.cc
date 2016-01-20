// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 22:49:34
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int NumWays(int n, int k) {
  std::vector<int> dp(n, 0);
  for (int i = 0; i < n; i++) {
    if (i == 0) dp[i] = k;
    else if (i == 1) dp[i] = k * k;
    else dp[i] = (k - 1) * (dp[i - 1] + dp[i - 2]);
  }
  return dp[n - 1];
}
}  // namespace algorithm

using namespace algorithm;

namespace clear {
int NumWays(int n, int k) {
  if (n <= 0) return 0;
  if (n == 1) return k;
  int same = k;
  int diff = k * (k - 1);
  for (int i = 2; i < n; i++) {
    int t = same;
    same = diff;
    diff = t * (k - 1) + diff * (k - 1);
  }
  return same + diff;
}
}  // namespace clear

int main(int argc, char** argv) {
  LOG(INFO) << NumWays(10, 4);
  LOG(INFO) << clear::NumWays(10, 4);
  return 0;
}
