// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 10:56:07
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 21/21 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 56 milli secs
 * Progress: 63/63 test cases passed
 *
 *
 * count the number of distinct subsequences of T in S.
 * 这句话有歧义吧
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

// dp[i][j] 的含义　s[0 ~ i]中有多少个不同的子序列 == t[0 ~ j]

void DP(std::string & s, std::string & t, std::vector<std::vector<int> > & dp) {
  int m = t.size();
  int n = s.size();
  for (int i = 0; i < n; i++) dp[0][i] = 1;
  for (int i = 1; i < m; i++) dp[i][0] = 0;

  for(int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (i > j) dp[i][j] = 0;
      else {
        if (t[i] == s[j]) dp[i][j] = dp[i - 1][j - 1] + dp[i][j - 1];
        else dp[i][j] = dp[i][j - 1];
      }
    }
  }
}

int NumDistinct(std::string & s, std::string t) {
  std::string ts = " " + s;
  std::string tt = " " + t;
  int m = tt.size();
  int n = ts.size();
  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  DP(ts, tt, dp);
  return dp[m - 1][n - 1];
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string s = "rabbbit";
  std::string t = "rabbit";
  LOG(INFO) << NumDistinct(s, t);
  return 0;
}
