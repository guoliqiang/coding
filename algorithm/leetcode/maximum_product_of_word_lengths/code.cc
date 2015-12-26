// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-25 12:58:08
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int MaxProduct(std::vector<std::string>& words) {
  int n = words.size();
  if (n == 0) return 0;
  std::vector<int> bit(n, 0);
  for (int i = 0; i < words.size(); i++) {
    for (int j = 0; j < words[i].size(); j++) {
      bit[i] |= (1 << (words[i][j] - 'a'));
    }
  }

  std::vector<int> dp(n, 0);
  for (int i = 1; i < n; i++) {
    dp[i] = dp[i - 1];
    for (int j = 0; j < i; j++) {
      if ((bit[i] & bit[j]) == 0) {
        dp[i] = std::max(dp[i], (int)words[i].size() * (int)words[j].size());
      }
    }
  }
  return dp[n - 1];
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::string> vec;
  vec.push_back("abcw");
  vec.push_back("baz");
  vec.push_back("foo");
  vec.push_back("bar");
  vec.push_back("xtfn");
  vec.push_back("abcdef");
  vec.push_back("abcw");
  LOG(INFO) << MaxProduct(vec);
  vec.clear();
  vec.push_back("a");
  vec.push_back("ab");
  vec.push_back("abc");
  vec.push_back("d");
  vec.push_back("cd");
  vec.push_back("bcd");
  vec.push_back("abcd");
  LOG(INFO) << MaxProduct(vec);
  vec.clear();
  vec.push_back("a");
  vec.push_back("aa");
  vec.push_back("aaa");
  vec.push_back("aaaa");
  LOG(INFO) << MaxProduct(vec);
  return 0;
}
