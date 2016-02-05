// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-02-01 18:04:41
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::string Gen(int len) {
  std::string ans = "122";
  std::string cnt = "12";
  while (ans.size() < len) {
    int cur = ans[cnt.size()] - '0';
    char val = ans[ans.size() - 1] == '1' ? '2' : '1';
    ans.append(cur, val);
    cnt.push_back(ans[cnt.size()]);
  }
  ans.resize(len);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << Gen(100);
  return 0;
}
