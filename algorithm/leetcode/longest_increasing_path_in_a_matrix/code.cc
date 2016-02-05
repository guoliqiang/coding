// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-21 14:28:08
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int dir [][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

int DFS(const std::vector<std::vector<int> > & matrix, int x, int y,
        std::vector<std::vector<int> > & dp) {
  int m = matrix.size();
  int n = matrix[0].size();
  if (dp[x][y] > 0) return dp[x][y];

  int max = 0;
  for (int i = 0; i < 4; i++) {
    int nx = x + dir[i][0];
    int ny = y + dir[i][1];

    if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
    if (matrix[nx][ny] <= matrix[x][y]) continue;
    max = std::max(max, DFS(matrix, nx, ny, dp));
  }
  dp[x][y] = max + 1;
  return dp[x][y];
}

int LongestIncreasingPath(std::vector<std::vector<int> >& matrix) {
  int m = matrix.size();
  if (m == 0) return 0;
  int n = matrix[0].size();

  int ans = 0;
  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      ans = std::max(ans, DFS(matrix, i, j, dp));
    }
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix(3, std::vector<int>(3, 0));
  matrix[0][0] = 9; matrix[0][1] = 9; matrix[0][2] = 4;
  matrix[1][0] = 6; matrix[1][1] = 6; matrix[1][2] = 8;
  matrix[2][0] = 2; matrix[2][1] = 1; matrix[2][2] = 1;
  LOG(INFO) << LongestIncreasingPath(matrix);
  return 0;
}
