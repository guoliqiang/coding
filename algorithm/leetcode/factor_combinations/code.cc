// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 13:18:55
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void Get(std::vector<int> & path, int v, std::vector<std::vector<int> > & ans) {
  if (v == 1) {
    ans.push_back(path);
    return;
  }

  for (int i = path.back(); i <= v; i++) {
    if (v % i == 0) {
      path.push_back(i);
      Get(path,  v / i, ans);
      path.pop_back();
    }
  }
}

std::vector<std::vector<int> > GetFactor(int n) {
  std::vector<std::vector<int> > ans;
  std::vector<int> path;
  for (int i = 2; i < n; i++) {
    if (n % i == 0) {
      path.push_back(i);
      Get(path, n / i, ans);
      path.pop_back();
    }
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > vec;
  vec = GetFactor(37);
  LOG(INFO) << JoinMatrix(&vec);
  LOG(INFO) << vec.size();
  return 0;
}
