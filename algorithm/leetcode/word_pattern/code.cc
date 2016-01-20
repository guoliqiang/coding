// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 17:01:15
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool WordPattern(const std::string & pattern, const std::string & str) {
  std::vector<std::string> vec;
  int b = -1;
  for (int i = 0; i < str.size(); i++) {
    if (str[i] == ' ') {
      vec.push_back(str.substr(b, i - b));
      b = -1;
    } else {
      if (b == -1) b = i;
    }
  }
  vec.push_back(str.substr(b, str.size() - b));

  if (vec.size() != pattern.size()) return false;
  std::map<char, int> p;
  std::map<std::string, int> s;

  for (int i = 0; i < pattern.size(); i++) {
    if (p.count(pattern[i]) != s.count(vec[i])) return false;
    if (p.count(pattern[i]) && p[pattern[i]] != s[vec[i]]) return false;

    if (!p.count(pattern[i])) {
      p[pattern[i]] = i;
      s[vec[i]] = i;
    }
  }
  return true;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << WordPattern("abab", "dog cat cat dog");
  return 0;
}
