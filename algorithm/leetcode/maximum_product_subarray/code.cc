// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-10-25 16:50:19
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
// 183 / 183 test cases passed.
// Status: Accepted
// Runtime: 20 ms
// Submitted: 0 minutes ago
int MaxProduct(int A[], int n) {
  std::vector<std::pair<int, int> > dp(n, std::make_pair(0, 0));
  int rs = 0;
  for (int i = 0; i < n; i++) {
    if (i == 0) {
      dp[i] = std::make_pair(A[i], A[i]);
      rs = dp[i].first;
    } else {
      int max = std::max(dp[i - 1].first * A[i], dp[i - 1].second * A[i]);
      max = std::max(max, A[i]);
      int min = std::min(dp[i - 1].first * A[i], dp[i - 1].second * A[i]);
      min = std::min(min, A[i]);
      dp[i] = std::make_pair(max, min);
      rs = std::max(rs, max);
    }
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  int A[] = {-2, -3, -2, 4, 2};
  LOG(INFO) << MaxProduct(A, 5);
  return 0;
}
