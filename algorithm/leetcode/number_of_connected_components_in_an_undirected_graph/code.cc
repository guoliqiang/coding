// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-08 12:17:15
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int Find(std::vector<int> & vec, int idx) {
  while (vec[idx] != idx) idx = vec[idx];
  return idx;
}

void Union(std::vector<int> & vec, int i, int j) {
  vec[i] = j;
}

int Num(int n, std::vector<std::pair<int, int> > & edges) {
  std::vector<int> vec(n, 0);
  int ans = n;
  for (int i = 0; i < n; i++) vec[i] = i;
  for (int i = 0; i < edges.size(); i++) {
    int idx1 = Find(vec, edges[i].first);
    int idx2 = Find(vec, edges[i].second);
    if (idx1 != idx2) {
      Union(vec, idx1, idx2);
      ans--;
    }
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::pair<int, int> > vec;
  vec.push_back(std::make_pair(0, 1));
  vec.push_back(std::make_pair(1, 2));
  vec.push_back(std::make_pair(3, 4));
  LOG(INFO) << Num(5, vec);
  vec.clear();
  vec.push_back(std::make_pair(0, 1));
  vec.push_back(std::make_pair(1, 2));
  vec.push_back(std::make_pair(2, 3));
  vec.push_back(std::make_pair(3, 4));
  LOG(INFO) << Num(5, vec);

  return 0;
}
