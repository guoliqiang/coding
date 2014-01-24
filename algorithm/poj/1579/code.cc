// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-25 05:35:47
// File  : code.cc
// Brief :

// Accepted 208K  0MS
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 22;
int N = 21;
int dp[MAXN][MAXN][MAXN];

void Dp() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) {
        if (i == 0 || j == 0 || k == 0) dp[i][j][k] = 1;
        else if (i < j && j < k) {
          dp[i][j][k] = dp[i][j][k - 1] + dp[i][j - 1][k - 1] - dp[i][j - 1][k];
        } else {
          dp[i][j][k] = dp[i - 1][j][k] + dp[i - 1][j - 1][k] + dp[i - 1][j][k - 1] - dp[i - 1][j - 1][k - 1];
        }
      }
    }
  }
}

void Read() {
  memset(dp, 0, sizeof(dp));
  Dp();
  int a, b, c;
  while (scanf("%d%d%d", &a, &b, &c) != EOF && (a != -1 || b != -1 || c != -1)) {
    int t = 0;
    if (a <= 0 || b <= 0 || c <= 0) t = 1;
    else if (a > 20 || b > 20 || c > 20) t = dp[20][20][20];
    else t = dp[a][b][c];
    printf("w(%d, %d, %d) = %d\n", a, b, c, t);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
