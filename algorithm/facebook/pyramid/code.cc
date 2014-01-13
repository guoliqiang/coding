// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-04 10:59:07
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
double KthCup(int k, int c, int l) {
  double rs = -1;
  std::vector<std::vector<double > >dp(2, std::vector<double>(k, 0));
  bool flag = false;
  dp[flag][0] = l;
  int num = 1;
  if (k == num) rs = dp[flag][0];
  else {
    int cnt = 2;
    while (true) {
      for (int i = 0; i < k; i++) dp[!flag][i] = 0;
      for (int i = 0; i < cnt; i++) {
        if (i == 0) {
          dp[!flag][i] = dp[flag][i] > c ? (dp[flag][i] - c) * 0.5 : 0;
        } else if (i == cnt - 1) {
          dp[!flag][i] = dp[flag][i - 1] > c ? (dp[flag][i - 1] - c) * 0.5 : 0;
        } else {
          dp[!flag][i] = dp[flag][i - 1] > c ? (dp[flag][i - 1] - c) * 0.5 : 0;
          dp[!flag][i] += dp[flag][i] > c ? (dp[flag][i] - c) * 0.5 : 0;
        }
        num++;
        if (num == k) rs = dp[!flag][i];
      }
      flag = !flag;
      cnt++;
      if (rs != -1) break;
    }
  }
  return rs > c ? c : rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << KthCup(6, 5, 10);
  return 0;
}
