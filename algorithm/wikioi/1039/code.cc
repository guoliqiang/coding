// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-18 10:19:49
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 203;
const int MAXK = 8;
int N;
int K;
int dp[MAXN][MAXK];

/*
不是第二类strling数，第二类strling数是集合的划分，集合内各个元素是不一样的。
dp[i][j] = dp[i - 1][j - 1]  // 划分的结果中至少有一份的个数是1
         + dp[i - j][j];     // 划分的结果中所有的都大于1
*/

int Dp() {
  memset(dp, 0, sizeof(dp));
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= std::min(i, K); j++) {
      if (j == 1 || i == j) dp[i][j] = 1;
      else {
        dp[i][j] = dp[i - 1][j - 1] + dp[i - j][j];
      }
    }
  }
  return dp[N][K];
}

void Read() {
  while (scanf("%d%d", &N, &K) != EOF) {
    int t = Dp();
    printf("%d\n", t);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
