// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 10:55:10
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::vector<std::string> GeneratePossibleNextMoves(const std::string & s) {
  std::vector<std::string> ans;
  for (int i = 0; i < s.size(); i++) {
    if (i > 0 && s[i] == '+' && s[i - 1] == '+') {
      ans.push_back(s.substr(0, i - 1) + "--" + s.substr(i + 1));
    }
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << JoinVector(GeneratePossibleNextMoves("++++"));
  return 0;
}
