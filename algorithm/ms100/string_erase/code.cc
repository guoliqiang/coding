// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-20 22:44:17
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void Erase(std::string & str, std::string & p) {
  std::set<char> t;
  for (int i = 0; i < p.size(); i++) t.insert(p[i]);
  int k = -1;
  for (int i = 0; i < str.size(); i++) {
    if (!t.count(str[i])) str[++k] = str[i];
  }
  str.resize(k + 1);
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  std::string str = "aabc";
  std::string p = "a";
  Erase(str, p);
  LOG(INFO) << str;
  LOG(INFO) << double();
  return 0;
}
