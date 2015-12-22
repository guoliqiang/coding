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
  }
  for (int i = 0; i < t.size(); i++) {
    if (vec[t[i] - 'a'] == 0) return false;
    vec[t[i] - 'a']--;
  }
  return true;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << IsAnagram("anagram", "nagaram");
  LOG(INFO) << IsAnagram("rat", "car");
  return 0;
}
