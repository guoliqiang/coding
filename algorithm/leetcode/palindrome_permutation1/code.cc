// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-10 11:44:35
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool CanPermutePalindrome(const std::string & s) {
  std::map<char, int> map;
  for (int i = 0; i < s.size(); i++) {
    if (!map.count(s[i])) map[s[i]] = 0;
    map[s[i]]++;
  }
  int cnt = 0;
  for (std::map<char, int>::iterator i = map.begin(); i != map.end(); i++) {
    if (i->second % 2) cnt++;
  }
  return cnt <= 1;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << CanPermutePalindrome("code");
  LOG(INFO) << CanPermutePalindrome("aab");
  LOG(INFO) << CanPermutePalindrome("carerac");
  return 0;
}
