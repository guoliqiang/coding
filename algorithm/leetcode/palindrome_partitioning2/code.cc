// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 11:10:14
// File  : code.cc
// Brief :

/*
Given a string s, partition s such that every substring of the partition is a palindrome.

Return the minimum cuts needed for a palindrome partitioning of s.

For example, given s = "aab",
Return 1 since the palindrome partitioning ["aa","b"] could be produced using 1 cut.

*/

#include "base/public/common_head.h"

namespace algorithm {

bool Palindrome(std::string & s) {
  if (s.size() <= 1) return true;
  int b = 0;
  int e = s.size() - 1;
  while (b <= e) {
    if (s[b] != s[e]) return false;
    else {
      b++;
      e--;
    }
  }
  return true;
}

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 12/12 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 1408 milli secs
 * Progress: 27/27 test cases passed.
 *
 * */

void DPF(std::string & str, std::vector<int> & dp) {
  dp[0] = 0;
  // 用于加速求解是不是回文串
  std::vector<std::vector<bool> > tdp(str.size(), std::vector<bool>(str.size(), false));
  
  for (int i = 1; i < str.size(); i++) {
    int min = 0x7fffffff;
    for (int k = 0; k <= i; k++) {
      std::string t = str.substr(k, i - k + 1);
      if (t.size() == 1 ||
         (t.size() == 2 && t[0] == t[1]) ||
         (t[0] == t[t.size() - 1] && tdp[k + 1][i - 1])) {
        tdp[k][i] = true;
        if (k == 0) {
          min = 0;
          // 不能break
        } else {
          min = std::min(min, dp[k - 1] + 1);
        }
      }
    }
    dp[i] = min;
  }
}

/*
 * 会 T
 * */
void DP(std::string & str, std::vector<int> & dp) {
  dp[0] = 0;
  for (int i = 1; i < str.size(); i++) {
    int min = 0x7fffffff;
    for (int k = 0; k <= i; k++) {
      std::string t = str.substr(k, i - k + 1);
      if (Palindrome(t)) {
        if (k == 0) {
          min = 0;
          break;
        } else {
          min = std::min(min, dp[k - 1] + 1);
        }
      }
    }
    dp[i] = min;
  }
}

int MinCut(std::string & str) {
  std::vector<int> dp(str.size(), 0);
  DPF(str, dp);
  return dp[str.size() - 1];
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str = "efe";
  LOG(INFO) << MinCut(str);
  return 0;
}
