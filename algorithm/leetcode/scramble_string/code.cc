// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 11:42:21
// File  : code.cc
// Brief :

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


int main(int argc, char** argv) {
  std::string str1 = "abc";
  std::string str2 = "bca";
  LOG(INFO) << str1 << " " << str2 << " " << IsScramble(str1, str2);
  LOG(INFO) << str1 << " " << str2 << " " << DP(str1, str2);
  LOG(INFO) << str1 << " " << str2 << " " << DPNew(str1, str2);
  return 0;
}
