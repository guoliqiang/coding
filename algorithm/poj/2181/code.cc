// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-19 00:15:31
// File  : code.cc
// Brief :

/*
 * Accepted  1868K  94MS
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 150002;
int N = 0;
int dp[MAXN][2];
int data[MAXN];

int DP() {
  memset(dp, 0, sizeof(dp));
  dp[0][0] = 0;
  dp[0][1] = data[0];
  for (int i = 1; i < N; i++) {
    dp[i][0] = std::max(dp[i - 1][0], dp[i - 1][1] - data[i]);
    dp[i][1] = std::max(dp[i - 1][1], dp[i - 1][0] + data[i]);
  }
  return std::max(dp[N - 1][0], dp[N - 1][1]);
}

void Read() {
  int p = 0;
  while (scanf("%d", &p) != EOF) {
    getchar();
    N = p;
    for (int i = 0; i < p; i++) {
      int t = 0;
      scanf("%d", &t);
      getchar();
      data[i] = t;
    }
    int t = DP();
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
