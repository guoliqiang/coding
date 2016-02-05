// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 22:49:34
// File  : code.cc
// Brief :

/*
There is a fence with n posts, each post can be painted with one of the k colors.

You have to paint all the posts such that no more than two adjacent fence posts have the same color.

Return the total number of ways you can paint the fence.

Note: n and k are non-negative integers.
*/

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

namespace clear2 {
int NumWays(int n, int k) {
  if (n <= 0) return 0;
  if (n == 1) return k;

  std::vector<int> same(n, 0);
  std::vector<int> diff(n, 0);
  for (int i = 1; i < n; i++) {
    if (i == 1) {
      same[i] = k;
      diff[i] = k * (k - 1);
    } else {
      same[i] = diff[i - 1];
      diff[i] = same[i - 1] * (k - 1) + diff[i - 1] * (k - 1);
    }
  }
  return same[n - 1] + diff[n - 1];
}
}  // namespace clear2

int main(int argc, char** argv) {
  LOG(INFO) << NumWays(10, 4);
  LOG(INFO) << clear::NumWays(10, 4);
  LOG(INFO) << clear2::NumWays(10, 4);
  return 0;
}
