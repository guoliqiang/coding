// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-25 11:09:40
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int NthSuperUglyNumber(int n, std::vector<int>& primes) {
  std::vector<int> idx(primes.size(), 0);
  std::vector<int> ans;
  ans.push_back(1);
  while (ans.size() < n) {
    int min = INT_MAX;
    for (int i = 0; i < idx.size(); i++) {
      min = std::min(min, primes[i] * ans[idx[i]]);
    }
    ans.push_back(min);
    for (int i = 0; i < idx.size(); i++) {
      if (ans.back() == ans[idx[i]] * primes[i]) idx[i]++;
    }
  }
  return ans.back();
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
