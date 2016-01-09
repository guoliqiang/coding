// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-08 21:11:03
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class NumMatrix {
 public:
  NumMatrix(std::vector<std::vector<int> > & matrix) {
    vec = matrix;
    int m = vec.size();
    int n = vec[0].size();

    std::vector<std::vector<int> > tmp(m, std::vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        if (j == 0) tmp[j][i] = matrix[j][i];
        else {
          tmp[j][i] = tmp[j - 1][i] + matrix[j][i];
        }
      }
    }
    sum.swap(tmp);
  }

  void update(int row, int col, int val) {
    int m = vec.size();
    for (int i = row; i < m; i++) {
      sum[i][col] +=(-vec[row][col] + val);
    }
  }

  int sumRegion(int row1, int col1, int row2, int col2) {
    int ans = 0;
    for (int i = col1; i <= col2; i++) {
      ans += sum[row2][i] - (row1 == 0 ? 0 : sum[row1 - 1][i]);
    }
    return ans;
  }
  std::vector<std::vector<int> > sum;
  std::vector<std::vector<int> > vec;
};
}  // namespace algorithm

using namespace algorithm;

namespace NB {
// 二维线段树/二维树状数组 log(n)
}  // namespace NB

int main(int argc, char** argv) {
  int matrix[][5] = {
  {3, 0, 1, 4, 2},
  {5, 6, 3, 2, 1},
  {1, 2, 0, 1, 5},
  {4, 1, 0, 1, 7},
  {1, 0, 3, 0, 5}
  };
  std::vector<std::vector<int> > vec(5, std::vector<int>(5, 0));
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      vec[i][j] = matrix[i][j];
    }
  }
  NumMatrix foo(vec);
  LOG(INFO) << foo.sumRegion(2, 1, 4, 3);
  foo.update(3, 2, 2);
  LOG(INFO) << foo.sumRegion(2, 1, 4, 3);
  return 0;
}
