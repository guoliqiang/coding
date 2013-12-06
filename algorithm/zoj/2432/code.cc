// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-06 18:21:49
// File  : code.cc
// Brief :

/*
Accepted  2432  C++ 40  1296
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 510;
int path[MAXN][MAXN];
int dp[MAXN];
int a[MAXN] = {0};
int b[MAXN] = {0};
int N = 0;
int M = 0;
int max = -1;
int index = -1;

void DP() {
  memset(path, 0, sizeof(path));
  memset(dp, 0, sizeof(dp));
  max = -1;
  index = -1;
  for (int i = 1; i < N; i++) {
    if (i != 1) {
      for (int j = 1; j < M; j++) {
        path[i][j] = path[i - 1][j];
      }
    }

    int k = 0;
    for (int j = 1; j < M; j++) {
      if (b[j] == a[i] && dp[k] + 1 > dp[j]) {
        dp[j] = dp[k] + 1;
        path[i][j] = (i + 1) * MAXN + k;
      } else {
        if (b[j] < a[i] && (dp[k] < dp[j])) k = j;
      }
    }

  }

  for (int i = 1; i < M; i++) {
    if (dp[i] > max) {
      max = dp[i];
      index = i;
    }
  }
}

void Trace(int x, int y) {
  if (y == 0) return;
  int t = path[x][y];
  int tx = t / MAXN - 1;
  int ty = t % MAXN;
  Trace(tx, ty);
  printf("%d ", b[y]);
}

void Trace() {
  if (max != 0) {
    Trace(N - 1, index);
    printf("\n");
  }
}

void Read() {
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int k = 0; k < c; k++) {
    for (int i = 0; i < 2; i++) {
      int n = 0;
      scanf("%d", &n);
      getchar();
      if (i == 0) N = n + 1;
      else M = n + 1;
      for (int j = 0; j < n; j++) {
        int t = 0;
        scanf("%d", &t);
        getchar();
        if (i == 0) a[j + 1] = t;
        else b[j + 1] = t;
      }
    }
    DP();
    printf("%d\n", max);
    Trace();
    printf("\n");
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
