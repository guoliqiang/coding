// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-16 14:26:37
// File  : code.cc
// Brief :

/*
Accepted  4084K 32MS
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1000;
int dp[MAXN][MAXN];
char str1[MAXN];
char str2[MAXN];


int Dp() {
  memset(dp, 0, sizeof(dp));
  int m = strlen(str1);
  int n = strlen(str2);
  if (m == 0 || n == 0) return 0;
  // 注意初始化的else部分
  for (int i = 0; i < n; i++) {
    if (str1[0] == str2[i]) dp[0][i] = 1;
    else if (i != 0) dp[0][i] = dp[0][i - 1];
  }
  for (int i = 0; i < m; i++) {
    if (str1[i] == str2[0]) dp[i][0] = 1;
    else if (i != 0) dp[i][0] = dp[i - 1][0];
  }
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (str1[i] == str2[j]) dp[i][j] = dp[i - 1][j - 1] + 1;
      else dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
    }
  }
  return dp[m - 1][n - 1];
}

// 这种写法不容易出bug
int Dp2() {
  memset(dp, 0, sizeof(dp));
  int m = strlen(str1);
  int n = strlen(str2);
  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      if (str1[i - 1] == str2[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
      else dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
    }
  }
  return dp[m][n];
}
void Read() {
  while (scanf("%s %s", str1, str2) != EOF) {
    int t = Dp2();
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
