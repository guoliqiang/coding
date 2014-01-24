// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-24 19:58:57
// File  : code.cc
// Brief :

// Accepted 208K  516MS

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 5008;
int N;
char data[MAXN];
int dp[2][MAXN];

int Dp() {
  memset(dp, 0, sizeof(dp));
  bool flag = false;
  dp[flag][0] = 0;
  for (int i = 1; i < N; i++) {
    for (int j = i; j >= 0; j--) {
      if (data[i] == data[j]) {
        if (i - j <= 1) dp[!flag][j] = 0;
        else dp[!flag][j] = dp[flag][j + 1];
      } else {
        dp[!flag][j] = std::min(dp[flag][j], dp[!flag][j + 1]) + 1;
      }
    }
    flag = !flag;
  }
  return dp[flag][0];
}

int DpMLE() {
  memset(dp, 0, sizeof(dp));
  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N - k; i++) {
      if (k == 0) dp[i][i + k] = 0;
      else {
        if (data[i] == data[i + k]) {
          dp[i][i + k] = k == 2 ? 0 : dp[i + 1][i + k - 1];
        } else {
          dp[i][i + k] = std::min(dp[i + 1][i + k], dp[i][i + k - 1]) + 1;
        }
      }
    }
  }
  return dp[0][N - 1];
};

void Read() {
  scanf("%d", &N);
  getchar();
  scanf("%s", data);
  int t = Dp();
  printf("%d\n", t);
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
