// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 17:26:16
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool IsAnagram(const std::string & s, const std::string & t) {
  if (s.size() != t.size()) return false;

  int vec[26] = {0};
  for (int i = 0; i < s.size(); i++) {
    vec[s[i] - 'a']++;
    vec[t[i] - 'a']--;
  }
  for (int i = 0; i < 26; i++) {
    if (vec[i] != 0) return false;
  }
  return true;
}

}  // namespace algorithm

namespace twice {

bool IsAnagram(std::string & s, std::string & t) {
  std::sort(s.begin(), s.end());
  std::sort(t.begin(), t.end());
  return s == t;
}
}  // namespace twice

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << IsAnagram("anagram", "nagaram");
  LOG(INFO) << IsAnagram("rat", "car");
  return 0;
}
