// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 10:56:07
// File  : code.cc
// Brief :


#include "base/public/common_head.h"

namespace algorithm {

int NumDistinct(std::string s, std::string t) {
  s = " " + s;
  t = " " + t;

  if (t.size() > s.size()) return 0;
  int m = t.size();
  int n = s.size();
  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) dp[0][i] = 1;

  for (int i = 1; i < m; i++) {
      for (int j = i; j < n; j++) {
          if (t[i] == s[j]) {
              dp[i][j] = (j - 2 >= 0 ? dp[i - 1][j - 2] : 1) + dp[i][j - 1];
          } else {
              dp[i][j] = dp[i][j - 1];
          }
      }
  }
  return dp[m - 1][n - 1];
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string s = "abcacb";
  std::string t = "ab";
  LOG(INFO) << s;
  LOG(INFO) << t;

  LOG(INFO) << NumDistinct(s, t);
  return 0;
}
