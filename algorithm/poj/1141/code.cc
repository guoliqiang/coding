// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-24 15:42:22
// File  : code.cc
// Brief :

// Accepted 2428K 313MS

#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 105;
int N;
char data[MAXN];
char dp[MAXN][MAXN][MAXN * 2];  // bug fixed * 2

void Dp() {
  memset(dp, 0, sizeof(dp));
  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N - k; i++) {
      if (k == 0) {
        if (data[i] == '(' || data[i] == ')') strcpy(dp[i][i + k], "()");
        else strcpy(dp[i][i + k], "[]");
      } else {
        for (int j = i; j < i + k; j++) {
          int n1 = strlen(dp[i][j]);
          int n2 = strlen(dp[j + 1][i + k]);

          int m = strlen(dp[i][i + k]);
          if (m == 0 || m > n1 + n2) {
            strcpy(dp[i][i + k], dp[i][j]);
            strcpy(&(dp[i][i + k][n1]), dp[j + 1][i + k]);
          }
        }

        if ((data[i] == '(' && data[i + k] == ')') ||
            (data[i] == '[' && data[i + k] == ']')) {
          if (k == 1) {
            strncpy(dp[i][i + k], &(data[i]), 2);
            dp[i][i + k][2] = '\0';
          } else {
            int n = strlen(dp[i + 1][i + k - 1]);
            int m = strlen(dp[i][i + k]);
            if (n + 2 < m || m == 0) {
              dp[i][i + k][0] = data[i];
              strcpy(&(dp[i][i + k][1]), dp[i + 1][i + k - 1]);
              dp[i][i + k][n + 1] = data[i + k];
              dp[i][i + k][n + 2] = '\0';
            }
          }
        }
      }
      // LOG(INFO) << "dp[" << i << "][" << i + k << "]:" << dp[i][i + k];
    }
  }
}

void Read() {
  gets(data);
  N = strlen(data);
  if (N != 0) {
    Dp();
    printf("%s\n", dp[0][N - 1]);
  } else {
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
