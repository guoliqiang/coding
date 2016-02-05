// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-22 10:31:52
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// 给一个string很长很长，找一个substring能不能除36余1，“agda4685766986579868687669686786786;?adfff7323”
// 有substring除36余1，substring只能有数字，但可以很长很长，可以远大于long的范
namespace algorithm {

bool Judge(const std::string & str) {
  int n = str.size();
  std::vector<std::set<int> > dp(n, std::set<int>());

  for (int i = 0; i < n; i++) {
    if (str[i] < '0' || str[i] > '9') continue;
    int cur = str[i] - '0';
    if (cur % 36 == 1) {
      LOG(INFO) << "find at " << i;
      return true;
    }
    dp[i].insert(cur % 36);

    if (i > 0) {
      std::set<int> & pre = dp[i - 1];
      for (std::set<int>::iterator j = pre.begin(); j != pre.end(); j++) {
        int t = ((*j) * 10 + cur) % 36;
        if (t == 1) {
          LOG(INFO) << "find at " << i;
          return true;
        }
        dp[i].insert(t);
      }
    }
  }
  return false;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << Judge("agda4685766986579868687669686786786;?adfff7323");
  return 0;
}
