// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 17:01:15
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

bool Match(const std::string & pattern, const std::string & words, int idx1, int idx2,
           std::map<char, int> & pdict, std::map<std::string, int> & wdict) {
  if (idx1 == pattern.size() && idx2 == words.size()) return true;
  if (idx1 == pattern.size() || idx2 == words.size()) return false;

  if (pdict.count(pattern[idx1])) {
    std::string cur;
    for (std::map<std::string, int>::iterator i = wdict.begin(); i != wdict.end(); i++) {
      if (i->second == pdict[pattern[idx1]]) cur = i->first;
    }
    if (cur.size() == 0) return false;

    if (words.substr(idx2, cur.size()) != cur) return false;
    return Match(pattern, words, idx1 + 1, idx2 + cur.size(), pdict, wdict);
  } else {
    for (int i = idx2; i < words.size(); i++) {
      std::string cur = words.substr(idx2, i - idx2 + 1);
      if (wdict.count(cur)) continue;

      pdict[pattern[idx1]] = idx1;
      wdict[cur] = idx1;
      if (Match(pattern, words, idx1 + 1, idx2 + cur.size(), pdict, wdict)) return true;
      pdict.erase(pattern[idx1]);
      wdict.erase(cur);
    }
    return false;
  }
}
bool WordPatternMatch(const std::string & pattern, const std::string & words) {
  std::map<char, int> pdict;
  std::map<std::string, int> wdict;
  return Match(pattern, words, 0, 0, pdict, wdict);
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << WordPatternMatch("abab", "catdogcatdof");
  return 0;
}
