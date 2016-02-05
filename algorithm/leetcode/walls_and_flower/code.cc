// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-02-01 17:31:15
// File  : code.cc
// Brief :

/*
0F00
00F0
0WF0
0F00
0 是点，F是花，W是墙， 你只能看到横竖两个方向的花，并且看不到墙后面的花
第一问：给一个点，找到能看到花的数目.
第二问：找到一个花园里可以看到最多花的点，返回一个list
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
int Num(std::vector<std::vector<char> > & matrix, int x, int y) {
  int m = matrix.size();
  int n = matrix[0].size();

  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));

  for (int i = 0; i < m; i++) {
    int max = 0;
    for (int j = 0; j < n; j++) {
      if (matrix[i][j] == 'F') max++;
      else if (matrix[i][j] == 'W') max = 0;
      dp[i][j] += max;
    }
    max = 0;
    for (int j = n - 1; j >= 0; j--) {
      if (matrix[i][j] == 'F') max++;
      else if (matrix[i][j] == 'W') max = 0;
      dp[i][j] += max;
    }
  }

  for (int i = 0; i < n; i++) {
    int max = 0;
    for (int j = 0; j < m; j++) {
      if (matrix[j][i] == 'F') max++;
      else if (matrix[j][i] == 'W') max = 0;
      dp[j][i] += max;
    }
    max = 0;
    for (int j = m - 1; j >= 0; j--) {
      if (matrix[j][i] == 'F') max++;
      else if (matrix[j][i] == 'W') max = 0;
      dp[j][i] += max;
    }
  }
  return dp[x][y];
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
