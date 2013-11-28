// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 14:11:18
// File  : code.cc
// Brief :

/*
Given two words word1 and word2, find the minimum number of steps required to convert word1 to word2. (each operation is counted as 1 step.)
You have the following 3 operations permitted on a word:

a) Insert a character
b) Delete a character
c) Replace a character

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 23/23 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 116 milli secs
 * Progress: 1145/1145 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {
void DP (std::vector<std::vector<int> > & dp, std::string & word1, std::string & word2) {
  int m = word1.size();
  int n = word2.size();
  // Note this initial means from "" to a string
  for (int i = 0; i < m; i++) dp[i][0] = i;
  for (int j = 0; j < n; j++) dp[0][j] = j;

  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      // delete and insert
      int tmin = std::min(dp[i - 1][j], dp[i][j - 1]) + 1;
      
      if (word1[i] == word2[j]) {
         // std::min(dp[i - 1][j], dp[i][j - 1]) + 1;  // delete and insert
        dp[i][j] = std::min(dp[i - 1][j - 1], tmin);
      } else {
        // dp[i - 1][j - 1] + 1;  // replace
        // std::min(dp[i - 1][j], dp[i][j - 1]) + 1;  // delete and insert
        dp[i][j] = std::min(dp[i - 1][j - 1] + 1 , tmin);
      }
    }
  }
}

int MinDistance(std::string word1, std::string word2) {
  if (word1.size() == 0 || word2.size() == 0)
    return std::max(word1.size(), word2.size());
 
  word1.insert(word1.begin(), ' ');
  word2.insert(word2.begin(), ' ');
  std::vector<std::vector<int> > dp(word1.size(), std::vector<int>(word2.size(), 0));
  DP(dp, word1, word2);
  return dp[word1.size() - 1][word2.size() - 1];
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
using namespace std;

int DP(string & word1, string & word2)  {
  word1 = " " + word1;
  word2 = " " + word2;
  int m = word1.size();
  int n = word2.size();
  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  dp[0][0] = 0;
  // 无论当前值想不想等都是从三个值中找最小的
  for (int i = 1; i < n; i++) dp[0][i] = i;
  for (int i = 1; i < m; i++) dp[i][0] = i;
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (word1[i] == word2[j])
        dp[i][j] = std::min(dp[i - 1][j - 1], std::min(dp[i - 1][j], dp[i][j - 1]) + 1);
      else
        dp[i][j] = std::min(dp[i - 1][j - 1] + 1, std::min(dp[i - 1][j], dp[i][j - 1]) + 1);
      }
  }
  return dp[m - 1][n - 1];
}

int DP2(std::string & word1, std::string & word2) {
  word1 = " " + word1;
  word2 = " " + word2;  // trick
  int m = word1.size();
  int n = word2.size();
  
  std::vector<std::vector<int> > dp(2, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) dp[0][i] = i;
  bool flag = false;
  
  for (int i = 1; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (j == 0) dp[!flag][j] = i;
      else {
        int t = std::min(dp[flag][j], dp[!flag][j - 1]) + 1;
        if (word1[i] == word2[j]) {
          dp[!flag][j] = std::min(dp[flag][j - 1], t);
        } else {
          dp[!flag][j] = std::min(dp[flag][j - 1] + 1, t);
        }
      }
    }
    flag = !flag;
  }
  return dp[flag][n - 1];
}
}  // namespace twice

int main(int argc, char** argv) {
  std::string word1;
  std::string word2;
  word1 = "guo";
  word2 = "go";

  LOG(INFO) << MinDistance(word1, word2);
  return 0;
}
