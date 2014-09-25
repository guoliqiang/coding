// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-26 11:07:48
// File  : code.cc
// Brief :

// Accepted 164K  0MS

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 50;
int N = 47;
int dp[MAXN][2];

void Dp() {
  memset(dp, 0, sizeof(dp));
  dp[0][0] = 1;
  dp[0][1] = 1;
  for (int i = 1; i < N; i++) {
    dp[i][0] = dp[i - 1][0] + dp[i - 1][1];
    dp[i][1] = dp[i - 1][0];
  }
}

void Read() {
  Dp();
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int i = 0; i < c; i++) {
    int v = 0;
    scanf("%d", &v);
    getchar();
    int t = dp[v - 1][0] + dp[v - 1][1];
    printf("Scenario #%d:\n%d\n\n", i + 1, t);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
