// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-17 16:57:51
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 12;
int N;
struct Node {
  int r;
  int l;
} data[MAXN];

long long dp[MAXN][MAXN];

void Dp() {
  memset(dp, 0, sizeof(dp));
  for (int k = 1; k < N; k++) {
    for (int i = 0; i < N - k; i++) {
      long long min = 0;
      for (int j = i + k; j > i; j--) {
        long long t = dp[j][i + k] + dp[i][j - 1] + (data[i].r * data[j].r * data[i + k].l);
        min = min == 0 ? t : std::min(t, min);
      }
      dp[i][i + k] = min;
    }
  }
}

void Trace(int b, int e) {
  if (b == e) {
    printf("A%d", b + 1);
  } else {
    printf("(");
    int mid = 0;
    for (int i = e ; i > b; i--) {
      if (dp[i][e] + dp[b][i - 1] + (data[b].r * data[i].r * data[e].l) == dp[b][e]) {
        mid = i;
        break;
      }
    }
    Trace(b, mid - 1);
    printf(" x ");
    Trace(mid, e);
    printf(")");
  }
}

void Read() {
  int num = 1;
  while (scanf("%d", &N) != EOF && N) {
    getchar();
    for (int i = 0; i < N; i++) {
      int r, l;
      scanf("%d%d", &r, &l);
      getchar();
      data[i].r = r;
      data[i].l = l;
    }
    Dp();
    printf("Case %d: ", num++);
    Trace(0, N - 1);
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
