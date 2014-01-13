// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 11:42:21
// File  : code.cc
// Brief :

/*
Given a string s1, we may represent it as a binary tree by partitioning it to two non-empty substrings recursively.
Below is one possible representation of s1 = "great":

    great
   /    \
  gr    eat
 / \    /  \
g   r  e   at
           / \
          a   t
To scramble the string, we may choose any non-leaf node and swap its two children.
For example, if we choose the node "gr" and swap its two children, it produces a scrambled string "rgeat".

    rgeat
   /    \
  rg    eat
 / \    /  \
r   g  e   at
           / \
          a   t
We say that "rgeat" is a scrambled string of "great".
Similarly, if we continue to swap the children of nodes "eat" and "at", it produces a scrambled string "rgtae".

    rgtae
   /    \
  rg    tae
 / \    /  \
r   g  ta  e
       / \
      t   a
We say that "rgtae" is a scrambled string of "great".
Given two strings s1 and s2 of the same length, determine if s2 is a scrambled string of s1.

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 78/78 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 364 milli secs
 * Progress: 281/281 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

void DPNew(std::string & str1, std::string & str2,
           std::vector<std::vector<std::vector<bool> > > & dp) {
  int n = str1.size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (str1[i] == str2[j]) dp[i][j][0] = true;
    }
  }
  
  // dp[i][j][k] str1[i] ~ str1[i + k]  str2[j] ~ str[j + k]
  for (int k = 1; k < n; k++) {
    for (int i = 0; i < n - k; i++) {
      for (int j = 0; j < n - k; j++) {
        for (int l = 0; l < k; l++) {
          // i ~ i+k 与 j～j+k, 从i或j走k步
          // 这个解释好
          if ((dp[i][j][l] && dp[i + l + 1][j + l + 1][k - l - 1]) ||
              (dp[i][j + k - l][l] && dp[i + l + 1][j][k - l - 1])) {
            dp[i][j][k] = true;
            break;
          }
        }
      }
    }
  }
}

bool DPNew(std::string & str1, std::string & str2) {
  if (str1.size() != str2.size()) return false;
  if (str1.size() == 0) return true;
  int n = str1.size();
  std::vector<std::vector<std::vector<bool> > >
  dp(n, std::vector<std::vector<bool> >(n, std::vector<bool>(n, false)));
  DPNew(str1, str2, dp);
  return dp[0][0][n - 1];
}



// 可读性不好
void DP(std::string & str1, std::string & str2,
        std::vector<std::vector<std::vector<bool> > > & dp) {
  int n = str1.size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (str1[i] == str2[j]) dp[i][j][0] = true;
    }
  }
  
  // dp[i][j][k] str1[i],str2[j]长度为k的串是否为scramble string
  for (int k = 2; k <= n; k++) {
    for (int i = 0; i < n - k + 1; i++) {
      for (int j = 0; j < n - k + 1; j++) {
        for (int l = 1; l < k; l++) {
          if ((dp[i][j][l - 1] && dp[i + l][j + l][k - l - 1]) ||
              (dp[i][j + k - l][l - 1] && dp[i + l][j][k - l - 1])) {
            dp[i][j][k - 1] = true;
            break;
          }
        }
      }
    }
  }
}

bool DP(std::string & str1, std::string & str2) {
  if (str1.size() != str2.size()) return false;
  if (str1.size() == 0) return true;
  int n = str1.size();
  std::vector<std::vector<std::vector<bool> > >
  dp(n, std::vector<std::vector<bool> >(n, std::vector<bool>(n, false)));
  DP(str1, str2, dp);
  return dp[0][0][n - 1];
}

// 会 T
bool IsScramble(std::string & str1, std::string & str2) {
  // LOG(INFO) << "compare:" << str1 << " with " << str2; 
  if (str1.size() != str2.size()) return false;
  if (str1 == str2) return true;
  if (str1.size() == 1) return false;

  int n = str1.size() - 1;
  for (int i = 1; i <= n; i++) {
    std::string f_1 = str1.substr(0, i);
    std::string l_1 = str1.substr(i);
    std::string f_2 = str2.substr(0, i);
    std::string l_2 = str2.substr(i);
    
    std::string rf_1 = str1.substr(str1.size() - i, i);
    std::string rl_1 = str1.substr(0, str1.size() - i);
    std::string rf_2 = str2.substr(str2.size() - i, i);
    std::string rl_2 = str2.substr(0, str2.size() - i);
    
    // LOG(INFO) << f_1 << " " << l_1 << " " << f_2 << " " << l_2;
    // LOG(INFO) << rf_1 << " " << rl_1 << " " << rf_2 << " " << rl_2;
    if ((IsScramble(f_1, f_2) && IsScramble(l_1, l_2)) ||
        (IsScramble(rf_1, f_2) && IsScramble(rl_1, l_2))) {
      // LOG(INFO) << str1 << " and " << str2 << " true";
      return true;
    }
  }
  // LOG(INFO) << str1 << " and " << str2 << " false";
  return false;
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
bool Dp(std::string & s1, std::string & s2) {
  if (s1.size() != s2.size()) return false;
  int n = s1.size();
  std::vector<std::vector<std::vector<int> > > dp(n, std::vector<std::vector<int> >(n, std::vector<int>(n, 0)));
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n - k; i++) {
      for (int j = 0; j < n - k; j++) {
        if (k == 0) dp[i][j][k] = s1[i] == s2[j] ? 1 : 0;
        else {
          for (int l = 0; l < k; l++) {
            if (dp[i][j][l] == 1 && dp[i + l + 1][j + l + 1][k - l - 1] == 1) dp[i][j][k] = 1;
            if (dp[i][j + k - l][l] == 1 && dp[i + l + 1][j][k - l - 1] == 1) dp[i][j][k] = 1;
          }
        }
      }
    }
  }
  return dp[0][0][n - 1];
}

// MLE
bool Dp2(std::string & s1, std::string & s2) {
  if (s1.size() != s2.size()) return false;
  int n = s1.size();
  std::vector<std::vector<std::vector<std::vector<int> > > > dp(n, std::vector<std::vector<std::vector<int> > >
  (n, std::vector<std::vector<int> >(n, std::vector<int>(n, 0))));
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n - k; i++) {
      for (int j = 0; j < n - k; j++) {
        if (k == 0) dp[i][i + k][j][j + k] = s1[i] == s2[j] ? 1 : 0;
        else {
          for (int l = 0; l < k; l++) {
            if (dp[i][i + l][j][j + l] == 1 && dp[i + l + 1][i + k][j + l + 1][j + k]) dp[i][i + k][j][j + k] = 1;
            if (dp[i][i + l][j + k - l][j + k] == 1 && dp[i + l + 1][i + k][j][j + k - l - 1] == 1) dp[i][i + k][j][j + k] = 1;
          }
        }
      }
    }
  }
  return dp[0][n - 1][0][n - 1];
}
}  // namespace twice

int main(int argc, char** argv) {
  std::string str1 = "abc";
  std::string str2 = "bca";
  LOG(INFO) << str1 << " " << str2 << " " << IsScramble(str1, str2);
  LOG(INFO) << str1 << " " << str2 << " " << DP(str1, str2);
  LOG(INFO) << str1 << " " << str2 << " " << DPNew(str1, str2);
  return 0;
}
