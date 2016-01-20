// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 16:06:45
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool IsStrobogrammatic(std::string num) {
  std::map<char, char> m;
  m['0'] = '0';
  m['1'] = '1';
  m['8'] = '8';
  m['6'] = '9';
  m['9'] = '6';
  int b = 0;
  int e = num.size() - 1;
  while (b <= e) {
    if (!m.count(num[b]) || !m.count(num[e]) ||
        m[num[b]] != num[e]) return false;
    b++;
    e--;
  }
  return true;
}
}  // namespace algorithm

namespace NB {
bool Check(std::string num) {
  int b = 0;
  int e = num.size() - 1;
  while (b <= e) {
    std::string cur = num.substr(b, 1) + num.substr(e, 1);
    if (cur == "00" || cur == "11" || cur == "88" || cur == "96" || cur == "69") {
      b++;
      e--;
    } else {
      return false;
    }
  }
  return true;
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << IsStrobogrammatic("818");
  LOG(INFO) << IsStrobogrammatic("828");
  return 0;
}
