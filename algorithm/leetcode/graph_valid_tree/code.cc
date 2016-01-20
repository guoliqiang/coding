// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 10:37:02
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int Find(std::vector<int> & vec, int k) {
  while (vec[k] != k) k = vec[k];
  return k;
}

void Union(std::vector<int> & vec, int a, int b) {
  vec[a] = b;
}

bool ValidTree(int n, std::vector<std::pair<int, int> > & pair) {
  std::vector<int> vec(n, 0);
  for (int i = 0; i < n; i++) vec[i] = i;
  int cnt = n;

  for (int i = 0; i < pair.size(); i++) {
    int x = Find(vec, pair[i].first);
    int y = Find(vec, pair[i].second);
    if (x == y) return false;
    Union(vec, x, y);
    cnt--;
  }
  return cnt == 1;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::pair<int, int> > vec;
  vec.push_back(std::make_pair(0, 1));
  vec.push_back(std::make_pair(0, 2));
  vec.push_back(std::make_pair(0, 3));
  vec.push_back(std::make_pair(1, 4));
  LOG(INFO) << ValidTree(5, vec);
  vec.clear();
  vec.push_back(std::make_pair(0, 1));
  vec.push_back(std::make_pair(1, 2));
  vec.push_back(std::make_pair(2, 3));
  vec.push_back(std::make_pair(1, 3));
  vec.push_back(std::make_pair(1, 4));
  LOG(INFO) << ValidTree(5, vec);
  return 0;
}
