// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-19 04:26:37
// File  : code.cc
// Brief :

/*
 * Accepted  268K  0MS
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 505;
int data1[MAXN];
int data2[MAXN];
int dp[MAXN];
int N1 = 0;
int N2 = 0;

int LCIS() {
  memset(dp, 0, sizeof(dp));
  for (int i = 0; i < N2; i++) {
    if (data2[i] == data1[0]) {
      dp[i] = 1;
    }
  }

  for (int i = 1; i < N1; i++) {
    int k = -1;
    for (int j = 0; j < N2; j++) {
      if (data2[j] == data1[i]) {
        dp[j] = k == -1 ? 1 : dp[k] + 1;
      }
      if (data2[j] < data1[i] && (k == -1 || dp[k] < dp[j])) k = j;
    }
  }
  int max = 0;
  for (int i = 0; i < N2; i++) max = std::max(max, dp[i]);
  return max;
}

void Read() {
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int i = 0; i < c; i++) {
    memset(data1, 0, sizeof(data1));
    memset(data2, 0, sizeof(data2));
    N1 = 0;
    N2 = 0;
    for (int j = 0; j < 2; j ++) {
      int n = 0;
      scanf("%d", &n);
      getchar();
      if (j == 0) N1 = n;
      else N2 = n;

      for (int k = 0; k < n; k++) {
        int t = 0;
        scanf("%d", &t);
        if (j == 0) data1[k] = t;
        else data2[k] = t;
      }
      getchar();
    }
    int rs = LCIS();
    printf("%d\n", rs);
    if (i != c - 1) printf("\n");
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
