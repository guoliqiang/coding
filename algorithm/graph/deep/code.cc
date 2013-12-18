// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-16 23:16:20
// File  : code.cc
// Brief :

// 有向无环图，求最长链的深度
// dp
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1000;
int N = 0;
int matrix[MAXN][MAXN];
int dp[MAXN];

int Deep(int i) {
  if (dp[i] != 0) return dp[i];
  dp[i] = 1;
  for (int k = 0; k < N; k++) {
    if (matrix[i][k] > 0) dp[i] = std::max(dp[i], Deep(k) + 1);
  }
  return dp[i];
}

int Deep() {
  memset(dp, 0, sizeof(dp));
  int rs = 0;
  for (int i = 0; i < N; i++) {
    rs = std::max(rs, Deep(i));
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
