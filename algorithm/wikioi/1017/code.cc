// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-18 09:44:41
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 42;
const int MAXK = 8;
int N;
int K;
long long dp[MAXK][MAXN];
char str[MAXN];
long long ToInt(int b, int e) {
  long long rs = 0;
  for (int i = b; i <= e; i++) {
    rs = rs * 10 + str[i] - '0';
  }
  return rs;
}

long long Dp() {
  memset(dp, 0, sizeof(dp));
  for (int i = 0; i < N; i++) dp[0][i] = ToInt(0, i);
  for (int i = 1; i <= K; i++) {
    for (int j = i; j < N; j++) {
      long long max = 0;
      for (int k = j; k > 0; k--) {
        max = std::max(max, dp[i - 1][k - 1] * ToInt(k, j));
      }
      dp[i][j] = max;
      // LOG(INFO) << "dp[" << i << "][" << j << "]:" << dp[i][j];
    }
  }
  return dp[K][N - 1];
}

void Read() {
  while (scanf("%d%d", &N, &K) != EOF) {
    getchar();
    scanf("%s", str);
    getchar();
    long long t = Dp();
    printf("%lld\n", t);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
