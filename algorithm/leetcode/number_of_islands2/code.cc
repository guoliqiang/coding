// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-08 21:58:30
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int dir[][4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

int Find(std::vector<int> & vec, int idx) {
  while (vec[idx] != idx) idx = vec[idx];
  return idx;
}

void Union(std::vector<int> & vec, int i, int j) {
  vec[i] = j;
}

std::vector<int> NumIslands2(int m, int n, std::vector<std::pair<int, int> >& positions) {
  int size = m * n;
  std::vector<int> vec(size, 0);
  std::vector<int> ans;
  std::vector<std::vector<int> > matrix(m, std::vector<int>(n, 0));

  for (int i = 0; i < size; i++) vec[i] = i;
  int cnt = 0;
  for (int i = 0; i < positions.size(); i++) {
    int a = positions[i].first;
    int b = positions[i].second;
    if (matrix[a][b] != 1) {
      matrix[a][b] = 1;
      cnt++;
      for (int i = 0; i < 4; i++) {
        int a2 = a + dir[i][0];
        int b2 = b + dir[i][1];
        if (a2 >= 0 && a2 < m && b2 >= 0 && b2 < n && matrix[a2][b2] == 1) {
          int k1 = Find(vec, a * n + b);
          int k2 = Find(vec, a2 * n + b2);
          if (k1 != k2) {
            cnt--;
            Union(vec, k1, k2);
          }
        }
      }
    }
    ans.push_back(cnt);
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::pair<int, int> > vec;
  vec.push_back(std::make_pair(0, 0));
  vec.push_back(std::make_pair(0, 1));
  vec.push_back(std::make_pair(1, 2));
  vec.push_back(std::make_pair(2, 1));
  vec.push_back(std::make_pair(1, 1));
  LOG(INFO) << JoinVector(NumIslands2(3, 3, vec));
  return 0;
}
