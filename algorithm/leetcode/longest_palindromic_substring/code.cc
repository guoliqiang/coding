// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-20 21:29:30
// File  : code.cc
// Brief :
#include "code.h"
using namespace algorithm;

namespace twice {
std::string Palindrome(std::string & s) {
  std::string str = "^";
  for (int i = 0; i < s.size(); i++) str+= "#" + s.substr(i, 1);
  str += "#$";
  std::vector<int> dp(str.size(), 0);
  dp[0] = 1;
  int cen = 0;
  int right = 0;
  for (int i = 1; i < str.size(); i++) {
    int mirror = 2 * cen - i;
    int left = 2 * cen - right;
    int t = mirror > left ? std::min(dp[mirror], mirror - left) : 0;  // 注意取一个最小值
    // int t = mirror > left ? dp[mirror] : 0; bug fixed
    while (str[i + t] == str[i - t]) t++;
    dp[i] = t;
    if (i + t - 1 > right) {
      right = i + t - 1;
      cen = i;
    }
  }
  int max = 0;
  int index = -1;
  for (int i = 0; i < str.size(); i++) {
    if (dp[i] > max) {
        max = dp[i];
        index = i;
    }
  }
  // 注意先找到起始位置
  // 起始位置不可能是#$，因为起dp值为1，max至少会是2
  int b = (index - max + 1) / 2;
  return s.substr(b, max - 1);
}
}  // namespace twice

int main(int argc, char** argv) {
  std::string str = "bb";
  LOG(INFO) << twice::Palindrome(str);
  // LOG(INFO) << LongestPalindrome(str);
  return 0;
}
