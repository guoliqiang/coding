// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-12 13:05:43
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

int Find(std::vector<std::vector<int> > & matrix,
         std::vector<std::vector<int> > & dis, int i, int j, int m, int n) {
  if (dis[i][j] == 0) {
    int cur = 0;
    if (i + 1 < m && matrix[i + 1][j] > matrix[i][j]) {
      cur = std::max(cur, Find(matrix, dis, i + 1, j, m, n));
    }
    if (j + 1 < n && matrix[i][j + 1] > matrix[i][j]) {
      cur = std::max(cur, Find(matrix, dis, i, j + 1, m, n));
    }
    if (i - 1 >= 0 && matrix[i - 1][j] > matrix[i][j]) {
      cur = std::max(cur, Find(matrix, dis, i - 1, j, m, n));
    }
    if (j - 1 >= 0 && matrix[i][j - 1] > matrix[i][j]) {
      cur = std::max(cur, Find(matrix, dis, i, j - 1, m, n));
    }
    dis[i][j] = 1 + cur;
  }
  return dis[i][j];
}

int Longest(std::vector<std::vector<int> > & matrix) {
  int ans = 0;
  int m = matrix.size();
  if (m == 0) return ans;
  int n = matrix[0].size();
  std::vector<std::vector<int> > dis(m, std::vector<int>(n, 0));

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      ans = std::max(ans, Find(matrix, dis, i, j, m, n));
    }
  }
  LOG(INFO) << JoinMatrix(&dis);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  int v[][10] = {
   {97, 47, 56, 36, 61, 31, 57, 54, 12, 55},
   {35, 57, 41, 13, 82, 80, 71, 93, 31, 62},
   {89, 36, 98, 75, 91, 46, 95, 53, 37, 99},
   {25, 45, 26, 17, 15, 82, 80, 73, 96, 17},
   {75, 22, 63, 96, 96, 36, 64, 31, 99, 86},
   {12, 80, 42, 74, 54, 14, 93, 17, 14, 55},
   {14, 15, 20, 71, 34, 50, 22, 60, 32, 41},
   {90, 69, 44, 52, 54, 73, 20, 12, 55, 52},
   {39, 33, 25, 31, 76, 45, 44, 84, 90, 52},
   {94, 35, 55, 24, 41, 63, 87, 93, 79, 24},
  };
  std::vector<std::vector<int> > matrix(10, std::vector<int>(10, 0));
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) matrix[i][j] = v[i][j];
  }
  LOG(INFO) << Longest(matrix);
  return 0;
}
