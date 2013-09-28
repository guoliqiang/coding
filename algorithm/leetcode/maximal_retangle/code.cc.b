// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 10:13:31
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

void DP(std::vector<std::vector<char> > & v, std::vector<std::vector<std::pair<int, int> > > & dp) {
  int m = v.size();
  int n = (m == 0) 0 : v[0].size();
  int max = 0;

  // first = col
  // second = row
  if (v[0][0] == '1') {
    dp[0][0].first = 1;
    dp[0][0].second = 1;
    max = 1;
  }
  for (int i = 1; i < n; i++) {
    if (v[0][i] == '1') {
      dp[0][i].first += dp[0][i - 1].first;
      dp[0][i].second = 1;
      max = std::max(max, dp[0][i].first * dp[0][i].second);
    } else {
      dp[0][i].first = 0;
      do[0][i].second = 0;
    }
  }

  for (int i = 1; i < m; i++) {
    if (v[i][0] == '1') {
      dp[i][0].second += dp[i - 1][0].second;
      dp[i][0].first = 1;
      max = std::max(max, dp[i][0].first * dp[i][0].second);
    } else {
      dp[i][0].first = 0;
      dp[i][0].second = 0;
    }
  }

  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (v[i][j] == '0') {
        dp[i][j].first = 0;
        dp[i][j].second = 0;
      } else {
      }
    }
  }

}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
