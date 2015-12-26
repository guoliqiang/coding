// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-25 11:36:10
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace NB {

std::string RemoveDuplicateLetters(const std::string & s) {
  std::vector<int> cnt(26, 0);
  std::vector<int> has(26, 0);
  std::string rs;

  for (int i = 0; i < s.size(); i++) {
    cnt[s[i] - 'a']++;
  }

  for (int i = 0; i < s.size(); i++) {
    cnt[s[i] - 'a']--;
    if (rs.size() == 0) {
      rs.push_back(s[i]);
      has[s[i] - 'a'] = 1;
    }
    if (has[s[i] - 'a'] == 1) continue;

    while (rs.size() > 0 && rs[rs.size() - 1] > s[i] &&
           cnt[rs[rs.size() - 1] - 'a'] > 0) {
      has[rs[rs.size() - 1] - 'a'] = 0;
      rs.resize(rs.size() - 1);
    }
    rs.push_back(s[i]);
    has[s[i] - 'a'] = 1;
  }
  return rs;
}

}  // namespace NB
//TLE
namespace algorithm {
std::string RemoveFirst(const std::string & str) {
  int idx = 0;
  for (int i = 1; i < str.size(); i++) {
    if (str[i] == str[0]) idx = i;
  }
  if (idx == 0) return str;
  if (str[1] < str[0]) return str.substr(1);
  else return str.substr(0, idx) + str.substr(idx + 1);
}

std::string RemoveLast(const std::string & str) {
  int idx = str.size() - 1;
  for (int i = str.size() - 2; i >= 0; i--) {
    if (str[i] == str[str.size() - 1]) idx = i;
  }
  if (idx == str.size() - 1) return str;
  if (str[idx + 1] < str[idx]) {
    return str.substr(0, idx) + str.substr(idx + 1);
  } else {
    return str.substr(0, str.size() - 1);
  }
}

std::string RemoveDuplicateLetters(const std::string & s) {
  int n = s.size();
  if (n == 0) return s;

  std::vector<std::vector<std::string> > dp(n, std::vector<std::string>(n, ""));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j + i < n; j++) {
      if (i == 0) dp[j][j + i].push_back(s[j]);
      else {
        std::string last = dp[j][j + i - 1] + std::string(1, s[j + i]);
        std::string first = std::string(1, s[j]) + dp[j + 1][j + i];
        last = RemoveLast(last);
        first = RemoveFirst(first);
        dp[j][j + i] = last < first ? last : first;
      }
    }
  }
  return dp[0][n - 1];
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << RemoveDuplicateLetters("bcabc");
  LOG(INFO) << NB::RemoveDuplicateLetters("bcabc");
  LOG(INFO) << RemoveDuplicateLetters("cbacdcbc");
  LOG(INFO) << NB::RemoveDuplicateLetters("cbacdcbc");
  return 0;
}
