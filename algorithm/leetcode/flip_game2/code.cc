// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 10:55:10
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool CanWin(const std::string & str, int cnt) {
  bool flag = false;
  for (int i = 0; i < str.size(); i++) {
    if (i > 0 && str[i] == '+' && str[i - 1] == '+') {
      flag = true;
      if (CanWin(str.substr(0, i - 1) + "--" + str.substr(i + 1), cnt + 1)) {
        return true;
      }
    }
  }
  if (flag) return false;
  else return cnt % 2;
}
}  // namespace algorithm

namespace NB {
bool CanWin(const std::string & str) {
  for (int i = 0; i < str.size(); i++) {
    if (i > 0 && str[i] == '+' && str[i - 1] == '+') {
      if (!CanWin(str.substr(0, i - 1) + "--" + str.substr(i + 1))) {
        return true;
      }
    }
  }
  return false;
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << NB::CanWin("++--++++");
  return 0;
}
