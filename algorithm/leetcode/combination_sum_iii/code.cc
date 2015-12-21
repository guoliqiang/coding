// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 20:45:13
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

void Sub(int k, int n, int cur, int sum, std::vector<int> & path,
    std::vector<std::vector<int> > & ans) {
  if (sum > n || path.size() > k) return;
  if (path.size() + 10 - cur < k) return;

  if (sum == n && path.size() == k) {
    ans.push_back(path);
  } else {
    for (int i = cur; i < 10; i++) {
      path.push_back(i);
      Sub(k, n, i + 1, sum + i, path, ans);
      path.pop_back();
    }
  }
}

std::vector<std::vector<int> > CombinationSum3(int k, int n) {
  std::vector<std::vector<int> > ans;
  std::vector<int> path;
  Sub(k, n, 1, 0, path, ans);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > ans = CombinationSum3(1, 9);
  for (int i = 0; i < ans.size(); i++) {
    LOG(INFO) << JoinVector(ans[i]);
  }
  return 0;
}
