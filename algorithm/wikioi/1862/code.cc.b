// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-22 17:41:59
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 5004;
int dp[MAXN][MAXN];
int num;
std::string str1;
std::string str2;

void Trace(int x, int y) {
  LOG(INFO) << "x:" << x << " y:" << y;
  // printf("%c", str1[x]);
  if (dp[x][y] == 1) {
    num = (num + 1) % 100000000;
    printf("\n");
  } else {
    for (int i = 1; i < x; i++) {
      for (int j = 1; j < y; j++) {
        if (str1[i] == str2[j] && dp[i][j] + 1 == dp[x][y]) Trace(i, j);
      }
    }
  }
}

void Trace() {
  num = 0;
  int m = str1.size() - 1;
  int n = str2.size() - 1;
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (str1[i] == str2[j] && dp[i][j] == dp[m - 1][n - 1]) {
        HERE(INFO);
        Trace(i, j);
      }
    }
  }
}

int Dp() {
  memset(dp, 0, sizeof(dp));
  int m = str1.size() - 1;
  int n = str2.size() - 1;
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (str2[j] == str1[i]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }
  return dp[m - 1][n - 1];
}

int dp2[MAXN][MAXN];
void Dp3() {
  num = 0;
  memset(dp2, 0, sizeof(dp2));
  int m = str1.size() - 1;
  int n = str2.size() - 1;
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (str1[i] == str2[j]) {
        dp2[i][j] = dp2[i - 1][j - 1] == 0 ? 1 : dp2[i - 1][j - 1];
        if (dp[i - 1][j] == dp[i][j]) dp2[i][j] += dp2[i - 1][j];
        if (dp[i][j - 1] == dp[i][j]) dp2[i][j] += dp2[i][j - 1];
      } else {
        dp2[i][j] = std::max(dp2[i - 1][j], dp2[i][j - 1]);
      }
    LOG(INFO) << "dp2[" << i << "][" << j << "]:" << dp2[i][j] << " dp[" << i << "][" << j << "]:" << dp[i][j];
    }
  }
  num = dp2[m - 1][n - 1];
}

void Dp2() {
  num = 0;
  memset(dp2, 0, sizeof(dp2));
  int m = str1.size() - 1;
  int n = str2.size() - 1;
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (str1[i] != str2[j]) continue;
      if (dp[i][j] == 1) {
        dp2[i][j] = 1;
      } else {
        int t = 0;
        for (int k = 1; k < i; k++) {
          for (int l = 1; l < j; l++) {
            if (str1[k] == str2[l] && dp[k][l] == dp[i][j] - 1) t += dp2[k][l];
          }
        }
        dp2[i][j] = t;
      }
      if (dp[i][j] == dp[m - 1][n - 1]) {
        num = (num + dp2[i][j]) % 100000000;
      }
    }
  }
}

void Read() {
  char s1[MAXN];
  char s2[MAXN];
  while (scanf("%s", s1) != EOF) {
    getchar();
    scanf("%s", s2);
    getchar();
    str1 = " " + std::string(s1);
    str2 = " " + std::string(s2);
    int t = Dp();
    Trace();
    Dp3();
    printf("%d\n%d\n", t, num);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
