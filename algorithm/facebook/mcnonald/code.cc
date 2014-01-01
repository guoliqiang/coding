// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-31 22:51:16
// File  : code.cc
// Brief :
// http://www.careercup.com/question?id=14974673

#include "base/public/common_ojhead.h"

namespace algorithm {
bool Dp(std::vector<int> & v, int n) {
  std::vector<int> dp(n + 1, -INF);
  dp[0] = 0;
  for (int i = 0; i < v.size(); i++) {
    for (int j = v[i]; j <= n; j++) {
      dp[j] = std::max(dp[j], dp[j - v[i]] + v[i]);
    }
  }
  return dp[n] > 0 ? true : false;
}


}  // namespace algorithm

namespace NB {
bool Trace(int n) {
  return n >= 0 && (n == 0 || n % 6 == 0 || n % 9 == 0 || n % 20 == 0 || Trace(n - 6) || Trace(n - 9) || Trace(n - 20));
}
}  // namespace NB

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(6);
  v.push_back(9);
  v.push_back(20);
  LOG(INFO) << Dp(v, 15);
  LOG(INFO) << Dp(v, 25);
  LOG(INFO) << Dp(v, 46);
  LOG(INFO) << Dp(v, 47);
  return 0;
}
