// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 17:19:28
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 12/12 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 44 milli secs
 * Progress: 43/43 test cases passed.
 *
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

int DP(std::vector<std::vector<int> > & v, std::vector<std::vector<int> > & dp) {
  bool flag = false;
  dp[flag][0] = v[0][0];
  for (int i = 1; i < v.size(); i++) {
    for (int j = 0; j <= i; j++) {
      if (j == 0) dp[!flag][j] = v[i][j] + dp[flag][j];
      else if (j == i) dp[!flag][j] = v[i][j] + dp[flag][j - 1];
      else dp[!flag][j] = std::min(dp[flag][j - 1], dp[flag][j]) + v[i][j];
    }
    // LOG(INFO) << JoinVector(dp[!flag]);
    flag = !flag;
  }
  // LOG(INFO) << JoinMatrix(&dp);
  return flag;
}

int MiniTotal(std::vector<std::vector<int> > & v) {
  if (v.size() == 0) return 0;
  std::vector<std::vector<int> > dp(2, std::vector<int>(v.size(), 0));
  int i = DP(v, dp);
  int rs = 0x7fffffff;
  for (int j = 0; j < v.size(); j++) {
    if (rs > dp[i][j]) rs = dp[i][j];
  }
  return rs;
}

}  // namespace algorithm

namespace twice {
int DP(std::vector<std::vector<int> > & matrix) {
  int n = matrix.size();
  for (int i = 1; i < n; i++) {
      for (int j = 0; j <= i; j++) {
          if (j == 0) matrix[i][j] += matrix[i - 1][j];
          else if (j == i) {
              matrix[i][j] += matrix[i - 1][j - 1];
          } else {
              matrix[i][j] += std::min(matrix[i - 1][j - 1], matrix[i - 1][j]);
          }
      }
  }
  int min = INT_MAX;
  for (int i = 0; i < n; i++) {
      min = std::min(min, matrix[n - 1][i]);
  }
  return min;
}
}  // namespace twice
using namespace algorithm;



int main(int argc, char** argv) {
  std::vector<std::vector<int> > v;
  std::vector<int> foo;
  foo.push_back(2);
  v.push_back(foo);
  foo.clear();
  foo.push_back(3);
  foo.push_back(4);
  v.push_back(foo);


  foo.clear();
  foo.push_back(6);
  foo.push_back(5);
  foo.push_back(7);
  v.push_back(foo);

  foo.clear();
  foo.push_back(4);
  foo.push_back(1);
  foo.push_back(8);
  foo.push_back(3);
  v.push_back(foo);

  LOG(INFO) << MiniTotal(v);
  return 0;
}
