// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 11:33:46
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int NthUglyNumber(int n) {
  std::set<long long int> s;
  s.insert(1);
  int ans = 0;
  for (int i = 0; i < n; i++) {
    std::set<long long int>::iterator it = s.begin();
    ans = *it;
    s.insert((*it) * 2);
    s.insert((*it) * 3);
    s.insert((*it) * 5);
    s.erase(it);
  }
  return ans;
}
}  // namespace algorithm

namespace NB {
int NthUglyNumber(int n) {
  std::vector<int> ans(1, 1);
  int idx2 = 0;
  int idx3 = 0;
  int idx5 = 0;
  while (ans.size() < n) {
    ans.push_back(std::min(ans[idx2] * 2,
      std::min(ans[idx3] * 3, ans[idx5] * 5)));
    if (ans.back() == ans[idx2] * 2) idx2++;
    if (ans.back() == ans[idx3] * 3) idx3++;
    if (ans.back() == ans[idx5] * 5) idx5++;
  }
  return ans[n - 1];
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << NthUglyNumber(1407);
  LOG(INFO) << NB::NthUglyNumber(1407);
  return 0;
}
