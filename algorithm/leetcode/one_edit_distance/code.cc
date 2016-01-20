// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-12 15:43:49
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool IsOneDifL(std::string s, std::string t) {
  // 必须要转换为int
  if (fabs((int)s.size() - (int)t.size()) >= 2) return false;
  if (s.size() == t.size()) {
    int cnt = 0;
    for (int i = 0; i < s.size(); i++) {
      cnt += s[i] == t[i] ? 0 : 1;
    }
    return cnt == 1;
  } else {
    if (s.size() > t.size()) std::swap(s, t);

    for (int i = 0; i < s.size(); i++) {
      if (s[i] != t[i]) {
        return s.substr(i) == t.substr(i + 1);
      }
    }
    return true;
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << IsOneDifL("ab", "abc");
  LOG(INFO) << IsOneDifL("ab", "ac");
  return 0;
}
