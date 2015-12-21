// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 12:08:39
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool IsIsomorphic(const std::string & s, const std::string & t) {
  if (s.size() != t.size()) return false;

  int cnt = 0;
  std::string s_internal;
  std::map<char, int> m;
  for (int i = 0; i < s.size(); i++) {
    if (!m.count(s[i])) {
      m[s[i]] = cnt++;
    }
    s_internal.push_back(m[s[i]]);
  }

  cnt = 0;
  m.clear();
  std::string t_internal;
  for (int i = 0; i < t.size(); i++) {
    if (!m.count(t[i])) {
      m[t[i]] = cnt++;
    }
    t_internal.push_back(m[t[i]]);
  }
  return s_internal == t_internal;
}

}  // namespace algorithm

namespace NB {
bool IsIsomorphic(const std::string & s, const std::string & t) {
  for (int i = 0; i < strlen (s.c_str()); i++) {
    if ((strchr (s.c_str(),s[i]) - s.c_str()) !=
        (strchr(t.c_str(),t[i]) - t.c_str())) return false;
  }
  return true;
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << IsIsomorphic("paper", "title")
            << NB::IsIsomorphic("paper", "title");
  LOG(INFO) << IsIsomorphic("bab", "abb")
            << NB::IsIsomorphic("bab", "abb");
  return 0;
}
