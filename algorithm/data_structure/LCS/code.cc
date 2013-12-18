// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-16 13:46:14
// File  : code.cc
// Brief :

// poj 1458
#include "base/public/common_ojhead.h"
namespace NB {

const int MAXN = 100;
int dp[MAXN][MAXN];
std::string rs;

void Dp(const char * str1, const char * str2) {
  int m = strlen(str1);
  int n = strlen(str2);
  memset(dp, 0, sizeof(dp));
  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      if (str1[i - 1]  == str2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }
}

void Trace(const char * str1, int m, const char * str2, int n) {
  if (m <= 0 || n <= 0) return;
  if (str1[m - 1] == str2[n - 1]) {
    Trace(str1, m - 1, str2, n - 1);
    rs.append(1, str1[m - 1]);
  } else {
    if (dp[m - 1][n] == dp[m][n]) Trace(str1, m - 1, str2, n);
    else Trace(str1, m, str2, n - 1);
  }
}

void Trace(const char * str1, const char * str2) {
  rs.clear();
  int m = strlen(str1);
  int n = strlen(str2);
  Trace(str1, m, str2, n);
}

void LCS(const char * str1, const char * str2) {
  Dp(str1, str2);
  Trace(str1, str2);
}

}  // namespace NB


namespace algorithm {
const int MAXN = 100;
int dp[MAXN][MAXN];
std::string rs;

// 这种写法容易出错
void Dp(const char * str1, const char * str2) {
  int m = strlen(str1);
  int n = strlen(str2);
  if (m == 0 || n == 0) return;
  memset(dp, 0, sizeof(dp));
  // 之前一直WA是因为初始化的时候没有else部分
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
      if (str1[i]  == str2[j]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }
}

void Trace(const char * str1, int m, const char * str2, int n) {
  if (m < 0 || n < 0) return;
  if (str1[m] == str2[n]) {
    Trace(str1, m - 1, str2, n - 1);
    rs.append(1, str1[m]);
  } else {
    if (dp[m - 1][n] == dp[m][n]) Trace(str1, m - 1, str2, n);
    else Trace(str1, m, str2, n - 1);
  }
}

void Trace(const char * str1, const char * str2) {
  rs.clear();
  int m = strlen(str1) - 1;
  int n = strlen(str2) - 1;
  Trace(str1, m, str2, n);
}

void LCS(const char * str1, const char * str2) {
  Dp(str1, str2);
  Trace(str1, str2);
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  const char * str1 = "acbd";
  const char * str2= "nacmd";
  LCS(str1, str2);
  LOG(INFO) << rs;
  NB::LCS(str1, str2);
  LOG(INFO) << NB::rs;
  return 0;
}
