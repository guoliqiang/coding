// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-30 13:34:42
// File  : code.cc
// Brief :

/*
A k-palindrome is a string which transforms into a palindrome on removing at most k characters. 

Given a string S, and an interger K, print "YES" if S is a k-palindrome; otherwise print "NO". 
Constraints: 
S has at most 20,000 characters. 
0<=k<=30 

Sample Test Case#1: 
Input - abxa 1 
Output - YES 
Sample Test Case#2: 
Input - abdxa 1 
Output - No
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
bool Dp(std::string str, int m) {
  LOG(INFO) << str << " " << m;
  int n = str.size();
  std::vector<std::vector<std::vector<int> > > dp(n, std::vector<std::vector<int> >(n, std::vector<int>(m + 1, 0)));
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n - k; i++) {
      for (int j = 0; j <= m; j++) {
        if (k == 0) dp[i][i + k][j] = 1;
        else {
          if (j > 0) dp[i][i + k][j] = dp[i][i + k][j - 1];
          dp[i][i + k][j] = std::max(dp[i][i + k][j], std::max(dp[i + 1][i + k][j - 1], dp[i][i + k - 1][j - 1]));
          if (k == 1) dp[i][i + k][j] = std::max(dp[i][i + k][j], str[i] == str[i + k] ? 1 : 0);
          else {
            if (str[i] == str[i + k]) dp[i][i + k][j] = std::max(dp[i][i + k][j], dp[i + 1][i + k - 1][j]);
          }
        }
      }
    }
  }
  return dp[0][n - 1][m];
}
}  // namespace algorithm

/*
串abxa通过删除于插入操作转换成axba多需要的最少操作数，如果小于2 * k则可以，如果大于2 * k则不可以
*/
namespace NB {
int Dp(std::string str1, std::string str2, int k) {
  // 可以使用滚动数组减少内存消耗
  LOG(INFO) << str1 << " " << str2;
  str1 = " " + str1;
  str2 = " " + str2;
  int n = str1.size();
  std::vector<std::vector<int> > dp(n, std::vector<int>(n, INF));
  for (int i = 0; i < n; i++) {
    dp[0][i] = dp[i][0] = i;
  }
  for (int i = 1; i < n; i++) {
    // 加速，因为最多操作2*k次
    // str2.size == str1.size 最终插入删除的次数一定是相同的
    int from = std::max(1, i - k);
    int to = std::min(n - 1, i + k);
    for (int j = from; j <= to; j++) {
      int t = std::min(dp[i - 1][j], dp[i][j - 1]) + 1;
      if (str1[i] == str2[j]) dp[i][j] = std::min(t, dp[i - 1][j - 1]);
      else dp[i][j] = t;
    }
  }
  return dp[n - 1][n - 1];
}
int Dp(std::string str, int k) {
  std::string str1 = str;
  std::string str2 = str;
  int b = 0;
  int e = str2.size() - 1;
  while (b < e) std::swap(str2[b++], str2[e--]);
  int t = Dp(str1, str2, k);
  LOG(INFO) << t;
  return t > 2 * k ? 0 : 1;
}
}  // 

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  LOG(INFO) << Dp("abxa", 1);
  LOG(INFO) << NB::Dp("abxa", 1);
  LOG(INFO) << Dp("abdxa", 2);
  LOG(INFO) << NB::Dp("abdxa", 2);

  return 0;
}
