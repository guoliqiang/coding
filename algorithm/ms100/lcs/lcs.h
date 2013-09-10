// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 19:26:39
// File  : lcs.h
// Brief :

#ifndef  __LCS_H_
#define  __LCS_H_

#include <string>
#include "base/public/logging.h"
#include "stdlib.h"
#include <algorithm>
#include <iostream>

namespace algorithm {

int lcs_sub(std::string & a, std::string & b, int ** dp) {
  for (int i = 0; i < a.size(); i++) {
    if (a[i] == b[0]) dp[i][0] = 1;
    else dp[i][0] = 0;
  }
  for (int i = 0; i < b.size(); i++) {
    if (a[0] == b[i]) dp[0][i] = 1;
    else dp[0][i] = 0;
  }
  for (int i = 1; i< a.size(); i++) {
    for (int j = 1; j < b.size(); j++) {
      if (a[i] != b[j]) {
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        // LOG(INFO) << "[" << i << "][" << j << "]" << dp[i][j];
      } else {
        int foo = dp[i - 1][j - 1];
        // LOG(INFO) << i << " " << j << "/" << a[i] << " " << b[j];
        dp[i][j] = std::max(foo + 1, std::max(dp[i - 1][j], dp[i][j - 1]));
        // LOG(INFO) << "[" << i << "][" << j << "]" << dp[i][j];
      }
      // LOG(INFO) << "[" << i << "][" << j << "]" << dp[i][j];
    }
  }
  return dp[a.size() - 1][b.size() - 1];
}

void print(std::string & a, std::string &b, int** dp, int i, int j) {
  if (i == 0 || j == 0) {
    if (dp[i][j]) std::cout << a[i];
    return;
  }
  if (dp[i][j] == dp[i - 1][j - 1] + 1 && a[i] == b[j]) {
    print(a, b, dp, i - 1, j - 1);
    std::cout << a[i];
  } else {
    if (dp[i][j] == dp[i][j - 1]) {
      print(a, b, dp, i, j - 1);
    } else {
      print(a, b, dp, i - 1, j);
    }
  }
}

void lcs_print(std::string & a, std::string &b) {
  int ** dp = new int *[a.size()];
  for (int i = 0; i < a.size(); i++) {
    dp[i] = new int[b.size()];
  }
  lcs_sub(a, b, dp);
  int i1 = a.size() - 1;
  int i2 = b.size() - 1;
  print(a, b, dp, i1, i2);
  std::cout << std::endl;
  for (int i = 0; i < a.size(); i++) {
    delete [] dp[i];
  }
  delete [] dp;
}

std::string lcs(std::string & a, std::string &b) {
  int ** dp = new int *[a.size()];
  for (int i = 0; i < a.size(); i++) {
    dp[i] = new int[b.size()];
  }
  lcs_sub(a, b, dp);
  int i1 = a.size() - 1;
  int i2 = b.size() - 1;
  std::string rs = "";
  while (i1 > 0 && i2 > 0) {
    if (dp[i1 - 1][i2 -1] + 1 == dp[i1][i2] &&
        a[i1] == b[i2]) {  // Note this two condition must used.
                           // e.g may dp[i1][i2] == dp[i1-1][i2] and
                           // dp[i1-1][i2] = dp[i1-1][i2 -1] +1
                           // this leading to a bug
      // LOG(INFO) << "[" << i1-1 << "][" << i2-1 << "]"
      //           << dp[i1 - 1][i2 -1] << "[" << i1 << "][" << i2 << "]" << dp[i1][i2];
      rs = a[i1] + rs;
      // LOG(INFO) << i1 << " " << i2 << "/" << a[i1] << " " << b[i2];
      i1--;
      i2--;
    } else {
      if (dp[i1][i2 - 1] == dp[i1][i2]) {
        i2--;
      } else {
        i1--;
      }
    }
  }
  if (dp[i1][i2]) rs = a[i1] + rs;
  for (int i = 0; i < a.size(); i++) {
    delete [] dp[i];
  }
  delete [] dp;
  return rs;
}

}  // namespace algorithm

#endif  //__LCS_H_
