// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-15 21:30:29
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void StringTest() {
  std::string str(10, '0');
  str.append(10, '1');
  for (int i = 0; i < str.size(); i++) LOG(INFO) << str[i];
  str.push_back('0');
  str.erase(str.size() - 1);
  // str.pop_back();
}
void StringTest2() {
  std::string str = "guoliqiang";
  CHECK(false);
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  StringTest2();
  return 0;
}
