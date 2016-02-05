// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-02-02 12:24:03
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// 求string s中含有string b order sequence 的最小长度
namespace algorithm {

int Min(const std::string & s, const std::string & t) {
  int m = t.size();
  int n = s.size();
  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) {
    dp[0][i] = t[0] == s[i] ? 1 : 0;
  }

  for (int i = 1; i < m; i++) {
    for (int j = i; j < n; j++) {
      if (t[i] == s[j]) {
        int t = INT_MAX;
        for (int k = i - 1; k < j; k++) {
          if (dp[i - 1][k] != 0) t = std::min(t, dp[i - 1][k] + j - k);
        }
        if (t != INT_MAX) dp[i][j] = t;
      }
    }
  }
  int min = INT_MAX;
  for (int i = m - 1; i < n; i++) {
   if (dp[m - 1][i] != 0) min = std::min(min, dp[m - 1][i]);
  }
  return min;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << Min("ADEFBDACBRFACGBC", "ABC");
  return 0;
}
