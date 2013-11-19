// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-18 20:57:35
// File  : code.cc
// Brief :

/*
 * Accepted  224K  0MS
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 101;
int N = MAXN;
double dp[MAXN][MAXN];

void Stirling2() {
  memset(dp, 0, sizeof(dp));
  for (int i = 1; i < N; i++) dp[i][1] = 1;
  for (int i = 2; i < N; i++) {
    for (int j = 2; j < N; j++) {
      dp[i][j] = dp[i - 1][j] * j + dp[i - 1][j - 1];
    }
  }
}

double Bell(int n) {
  double rs = 0;
  for (int i = 1; i <= n; i++) {
    rs += dp[n][i];
  }
  return rs;
}

void Read() {
  Stirling2();
  int n = 0;
  while (scanf("%d", &n) && n) {
    double t = Bell(n);
    printf("%d %.0f\n", n, t);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
