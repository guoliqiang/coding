// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 10:44:41
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int TitleToNumber(const std::string & s) {
  int ans = 0;
  for (int i = 0; i < s.size(); i++) {
    ans = ans * 26 + (s[i] - 'A' + 1);
  }
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << TitleToNumber("AA");
  return 0;
}
