// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 01:44:30
// File  : code.cc
// Brief :

/*
Given s1, s2, s3, find whether s3 is formed by the interleaving of s1 and s2.

For example,
Given:
s1 = "aabcc",
s2 = "dbbca",

When s3 = "aadbbcbcac", return true.
When s3 = "aadbbbaccc", return false.



*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 48/48 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 99/99 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

void DP(std::string & str1, std::string & str2, std::string & inter,
        std::vector<std::vector<int> > & dp) {
  int m = str1.size();
  int n = str2.size();
  dp[0][0] = 1;
  for (int i = 1; i < m; i++) {
    if (str1.substr(1, i) == inter.substr(0, i)) dp[i][0] = 1;
  }
  for (int i = 1; i < n; i++) {
    if (str2.substr(1, i) == inter.substr(0, i)) dp[0][i] = 1;
  }
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (inter[i + j - 1] == str1[i]) dp[i][j] = dp[i - 1][j];
      if (dp[i][j] != 1 && inter[i + j - 1] == str2[j]) dp[i][j] = dp[i][j - 1];
    }
  }
  // LOG(INFO) << JoinMatrix(&dp);
}

bool DP(std::string & str1, std::string & str2, std::string & inter) {
  if (str1.size() + str2.size() != inter.size()) return false;
  std::string tstr1 = " " + str1;
  std::string tstr2 = " " + str2;
  std::vector<std::vector<int> > dp(tstr1.size(), std::vector<int>(tstr2.size(), 0));
  DP(tstr1, tstr2, inter, dp);
  return dp[tstr1.size() - 1][tstr2.size() - 1] == 1;
}

// 会 T
bool InterleavingString(const std::string & str1, const std::string & str2,
                        const std::string & inter) {
  if (str1.size() + str2.size() != inter.size()) return false;
  if (str1.size() == 0 && str2.size() == 0 && inter.size() == 0) return true;
  for (int i = 0; i < str1.size(); i++) {
    if (str1[i] == inter[i] && InterleavingString(str1.substr(i + 1), str2, inter.substr(i + 1)))  return true;
    else break;
  }
  for (int i = 0; i < str2.size(); i++) {
    if (str2[i] == inter[i] && InterleavingString(str1, str2.substr(i + 1), inter.substr(i + 1))) return true;
    else break;
  }
  return false;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str1 = "aabcc";
  std::string str2 = "dbbca";
  std::string inter = "aadbbcbcac";
  LOG(INFO) << str1 << " " << str2 << " " << inter;
  LOG(INFO) << InterleavingString(str1, str2, inter);
  LOG(INFO) << DP(str1, str2, inter);
  inter = "aadbbbaccc";
  LOG(INFO) << str1 << " " << str2 << " " << inter;
  LOG(INFO) << InterleavingString(str1, str2, inter);
  LOG(INFO) << DP(str1, str2, inter);
  
  return 0;
}
