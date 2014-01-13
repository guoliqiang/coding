// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-11 16:05:24
// File  : code.cc
// Brief :

/*
Given a string s and a dictionary of words dict, determine if s can be segmented into a space-separated sequence of one or more dictionary words.
For example, given
s = "leetcode",
dict = ["leet", "code"].
Return true because "leetcode" can be segmented as "leet code".
*/

#include "base/public/common_head.h"
#include <unordered_set>
namespace algorithm {
/*
 *
 * 19 / 19 test cases passed.
 * Status: Accepted
 * Runtime: 16 ms
 * Submitted: 0 minutes ago
 *
 * */
bool DP(const std::string & s, std::unordered_set<std::string> & dict) {
  int len = 0;
  for (std::unordered_set<std::string>::iterator i = dict.begin(); i != dict.end(); i++) {
    len = std::max(len, (int)i->size());
  }
  std::vector<bool>dp(s.size(), false);
  for (int i = 0; i < s.size(); i++) {
    for (int j = i; j >= std::max(0, i - len - 1); j--) {
      std::string t = s.substr(j, i - j + 1);
      if (j - 1 >= 0) dp[i] = dict.count(t) ? dp[j - 1] : false;
      else dp[i] = dict.count(t) ? true : false;
      if (dp[i]) break;
    }
  }
  return dp[s.size() - 1];
}

/*
 * 会T
 * 直接深度优先搜索会T
 * */
bool WordBreak(const std::string & s, std::unordered_set<std::string> & dict) {
  if (dict.count(s)) return true;
  for (int i = 0; i < s.size() - 1; i++) {
    const std::string & t = s.substr(0, i + 1);
    if (dict.count(t) && WordBreak(s.substr(i + 1), dict)) return true;
  }
  return false;
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
bool  WordBreak(std::string s, std::unordered_set<std::string> & dict) {
  s = " " + s;
  int n = s.size();
  std::vector<int> dp(n, 0);
  int wsize = 0;
  for (std::unordered_set<std::string>::iterator i = dict.begin(); i != dict.end(); i++) {
    wsize = std::max(wsize, (int)(i->size()));
  }
  dp[0] = 1;
  for (int i = 1; i < n; i++) {
    for (int j = 1; j <= std::min(wsize, i) && dp[i] == 0; j++) {
      std::string t = s.substr(i - j + 1, j);
      if (dict.count(t)) dp[i] = dp[i - j];
    }
  }
  return dp[n - 1];
}

}  // namespace twice

int main(int argc, char** argv) {
  std::unordered_set<std::string> dict;
  dict.insert("leet");
  dict.insert("code");
  std::string s = "leetcode";
  LOG(INFO) << WordBreak(s, dict);
  LOG(INFO) << DP(s, dict);
  return 0;
}
