// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 15:18:40
// File  : p26.h
// Brief :

#ifndef  __P26_H_
#define  __P26_H_

#include "base/public/logging.h"

namespace algorithm {

void DP(int m, int n, int **dp) {  // n>=m
  for (int i = 0; i < m; i++) {
    dp[i][0] = 0;
  }
  for (int i = 0; i < n; i++) {
    dp[0][n] = 0;
  }
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (dp[i - 1][j] + i <= j) {
        dp[i][j] = dp[i - 1][j] + i;
      } else {
        if (i > j) dp[i][j] = 0;
        else dp[i][j] = i + dp[i - 1][j - i];
      }
      // LOG(INFO) << "dp[" << i <<"][" << j <<"]" << dp[i][j];
    }
  }
}

void p26(int m) {
  m++;
  int ** dp = new int* [m];
  for (int i = 0; i < m; i++) {
    dp[i] = new int[m];
  }
  DP(m, m, dp);
  for (int i = 0; i < m; i++) {
    if (dp[i][m - 1]  == m - 1) {
      LOG(INFO) << "find:" << i;
    }
  }
  for (int i = 0; i < m; i++) {
    delete [] dp[i];
  }
  delete [] dp;
}

int sum(int b, int e) {
  int rs = 0;
  for (int i = b; i <= e; i++) rs+=i;
  return rs;
}

void p26_v2_sub(int b, int e, int n) {
  while (b <= e) {
    if (sum(b, e) < n) b++;
    else if (sum(b, e) > n) e--;
    else {
      LOG(INFO) << e;
      return;
    }
  }
}

void p26_v2(int m) {
  // int foo = (m + 1) / 2;
  for (int i = 1; i <=m;  i++) {
    p26_v2_sub(i, m, m);
  }
}


}  // namespace algorithm

#endif  //  __P26_H_
