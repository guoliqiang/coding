// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 18:40:02
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::string ShortestPalindrome(const std::string & s) {
  int n = s.size();
  if (n == 0) return s;

  std::vector<std::vector<int> > dp(n, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n - i; j++) {
      if (i == 0) dp[j][j + i] = 1;
      else if (i == 1) {
        if (s[j] == s[j + i]) dp[j][j + i] = 1;
      } else {
        if (dp[j + 1][j + i - 1] == 1 && s[j] == s[i + j]) {
          dp[j][j + i] = 1;
        }
      }
    }
  }

  int index = 0;
  for (int i = 0; i < n; i++) {
    if (dp[0][i] == 1) {
      index = i;
    }
  }
  std::string ans = s;
  for (int j = index + 1; j < n; j++) {
    ans.insert(ans.begin(), s[j]);
  }
  return ans;
}

}  // namespace algorithm

namespace NB {
std::string ShortestPalindrome(const std::string & s) {
  if (s.size() == 0) return s;

  std::string str = s;
  str.push_back('#');
  for (int i = s.size() - 1; i >= 0; i--) {
    str.push_back(s[i]);
  }
  int n = str.size();
  std::vector<int> next(n, 0);
  next[0] = -1;
  for (int i = 1; i < n; i++) {
    int idx = next[i - 1];
    while (idx >= 0 && str[i - 1] != str[idx]) {
      idx = next[idx];
    }
    idx++;
    next[i] = idx;
  }
  LOG(INFO) << str;
  LOG(INFO) << JoinVector(next);

  std::string ans = s;
  for (int i = next[n - 1] + 1; i < s.size(); i++) {
    ans.insert(ans.begin(), s[i]);
  }
  return ans;
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << NB::ShortestPalindrome("abc");
  return 0;
}
