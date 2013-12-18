// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-16 18:21:36
// File  : code.cc
// Brief :

// Accepted 256K  204MS
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 510;
const int MAXW = 10005;
int N;
int W;
int p[MAXN];
int w[MAXN];
int dp[MAXW][2];

// Accepted 220K  79MS
// 滚动数组
int dp2[MAXW];
int Dp2() {
  dp2[0] = 0;
  for (int i = 1; i <= W; i++) dp2[i] = INF;
  for (int j = 1; j <= N; j++) {
    for (int i = w[j]; i <= W; i++) {
      dp2[i] = std::min(dp2[i], dp2[i - w[j]] + p[j]);
    }
  }
  return dp2[W];
}

int Dp() {
  memset(dp, 0, sizeof(dp));
  bool flag = false;
  for (int j = 1; j <= N; j++) {
    for (int i = 0; i <= W; i++) {
      dp[i][!flag] = dp[i][flag];
      if (i >= w[j]) {
        if (i == w[j]) dp[i][!flag] = dp[i][!flag] == 0 ? p[j] : std::min(dp[i][!flag], p[j]);
        else if (dp[i - w[j]][!flag] > 0) {
          int t = dp[i - w[j]][!flag] + p[j];
          dp[i][!flag] = dp[i][!flag] == 0 ? t : std::min(dp[i][!flag], t);
        }
      }
    }
    flag = !flag;
  }
  return dp[W][flag];
}

void Read() {
  int T = 0;
  while (scanf("%d", &T) != EOF) {
    getchar();
    for (int k = 0; k < T; k++) {
      int t1, t2;
      scanf("%d%d", &t1, &t2);
      getchar();
      W = t2 - t1;
      scanf("%d", &N);
      memset(p, 0, sizeof(p));
      memset(w, 0, sizeof(w));
      for (int i = 0; i < N; i++) {
        scanf("%d%d", &t1, &t2);
        getchar();
        p[i + 1] = t1;
        w[i + 1] = t2;
      }
      // int t = Dp();
      // if (t == 0) printf("This is impossible.\n");
      int t = Dp2();
      if (t == INF) printf("This is impossible.\n");
      else printf("The minimum amount of money in the piggy-bank is %d.\n", t);
    }
  }
}


}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
