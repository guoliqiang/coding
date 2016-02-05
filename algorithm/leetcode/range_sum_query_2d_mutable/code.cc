// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-08 21:11:03
// File  : code.cc
// Brief :

// 注意第0行第0列是不用的
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
    vec[row][col] = val;
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
// 二维线段树/二维树状数组 log(m) * log (n)
class NumMatrix {
 public:
  int Low(int k) {
    // return k & (k ^ (k - 1));
    return k & (-k);
  }

  NumMatrix(std::vector<std::vector<int> > & matrix) {
    vec = matrix;
    m = vec.size();
    n = vec[0].size();

    std::vector<std::vector<int> > sum1(m + 1, std::vector<int>(n + 1, 0));
    std::vector<std::vector<int> > sum2(m + 1, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
        int t = 0;
        for (int k = j - Low(j) + 1; k <= j; k++) t += matrix[i - 1][k - 1];
        sum1[i][j] = t;
      }
    }
    for (int j = 1; j <= n; j++) {
      for (int i = 1; i <= m; i++) {
        int t = 0;
        for (int k = i - Low(i) + 1; k <= i; k++) t += sum1[k][j];
        sum2[i][j] = t;
      }
    }
    sum.swap(sum2);
  }

  void update(int row, int col, int val) {
    int v = -vec[row][col] + val;
    vec[row][col] = val;
    update2(row + 1, col + 1, v);
  }

  void update2(int x, int y, int v) {
    for (int i = x; i <= m; i += Low(i)) {
      for (int j = y; j <= n; j += Low(j)) {
        sum[i][j] += v;
      }
    }
  }

  int sumRegion(int row1, int col1, int row2, int col2) {
    int ans = sum2(row2 + 1, col2 + 1);
    if (row1 - 1 >= 0 && col1 - 1 >= 0) {
      ans += sum2(row1, col1);
    }
    if (col1 - 1 >= 0) ans -= sum2(row2 + 1, col1);
    if (row1 - 1 >= 0) ans -= sum2(row1, col2 + 1);
    return ans;
  }

  int sum2(int x, int y) {
    int t = 0;
    for (int i = x; i > 0; i -= Low(i)) {
      for (int j = y; j > 0; j -= Low(j)) {
        t += sum[i][j];
      }
    }
    return t;
  }

  int m;
  int n;
  std::vector<std::vector<int> > sum;
  std::vector<std::vector<int> > vec;
};

}  // namespace NB

namespace clear {
class NumMatrix {
 public:
  int Low(int i) {
    return i & (-i);
  }

  NumMatrix(std::vector<std::vector<int> > & matrix) {
    m = matrix.size();
    n = matrix[0].size();
    sum.resize(m + 1, std::vector<int>(n + 1, 0));
    val.resize(m + 1, std::vector<int>(n + 1, 0));
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        update(i, j, matrix[i][j]);
      }
    }
  }

  void update(int x, int y, int v) {
    x += 1;
    y += 1;
    int diff = v - val[x][y];
    val[x][y] = v;
    for (int i = x; i <= m; i += Low(i)) {
      for (int j = y; j <= n; j += Low(j)) {
        sum[i][j] += diff;
      }
    }
  }

  int Range(int x, int y) {
    x += 1;
    y += 1;
    int ans = 0;
    for (int i = x; i > 0; i -= Low(i)) {
      for (int j = y; j > 0; j -= Low(j)) {
        ans += sum[i][j];
      }
    }
    return ans;
  }

  int sumRegion(int row1, int col1, int row2, int col2) {
    int ans = Range(row2, col2) - Range(row2, col1 - 1) - Range(row1 - 1, col2) + Range(row1 - 1, col1 - 1);
    return ans;
  }


  int m;
  int n;
  std::vector<std::vector<int> > sum;
  std::vector<std::vector<int> > val;
};
}  // namespace clear

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
  NB::NumMatrix foo2(vec);
  LOG(INFO) << JoinMatrix(&vec);
  clear::NumMatrix foo3(vec);
  LOG(INFO) << foo.sumRegion(2, 1, 4, 3);
  LOG(INFO) << foo2.sumRegion(2, 1, 4, 3);
  LOG(INFO) << foo3.sumRegion(2, 1, 4, 3);
  foo.update(3, 2, 2);
  foo2.update(3, 2, 2);
  foo3.update(3, 2, 2);
  LOG(INFO) << foo.sumRegion(2, 1, 4, 3);
  LOG(INFO) << foo2.sumRegion(2, 1, 4, 3);
  LOG(INFO) << foo3.sumRegion(2, 1, 4, 3);

  foo.update(3, 2, -9);
  foo2.update(3, 2, -9);
  foo3.update(3, 2, -9);
  LOG(INFO) << foo.sumRegion(2, 1, 4, 3);
  LOG(INFO) << foo2.sumRegion(2, 1, 4, 3);
  LOG(INFO) << foo3.sumRegion(2, 1, 4, 3);
  return 0;
}
