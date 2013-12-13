// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-10 12:54:21
// File  : floyd.cc
// Brief :

#include "base/public/common_ojhead.h"
#include "../base/graph.h"

namespace algorithm {
const int MAXN = 1000;
int N;
int matrix[MAXN][MAXN];
int dis[MAXN][MAXN];
int path[MAXN][MAXN];

void Floyd() {
  // 初始化
  memset(path, 0, sizeof(path));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      dis[i][j] = matrix[i][j] == 0 ? INF : matrix[i][j];
      if (dis[i][j] != INF) path[i][j] = i;
    }
  }

  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (dis[i][j] > dis[i][k] + dis[k][j]) {
          dis[i][j] = dis[i][k] + dis[k][j];
          path[i][j] = path[k][j];
        }
      }
    }
  }
}

}  // namesapce algorithm

namespace algorithm {
// if rs[i][j] == INT_MAX then i，j之间没有通路
void Floyd(std::vector<std::vector<int> > & matrix, std::vector<std::vector<int> > & rs) {
  int m = matrix.size();
  int n = (m == 0) ? 0 : matrix[0].size();
  rs.resize(m, std::vector<int>(n, INT_MAX));
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (matrix[i][j] > 0) rs[i][j] = matrix[i][j];
    }
  }
  // 实际上是DP
  // O(n ^ 3)
  for (int k = 0; k < n; k++) {  // 循环中间节点必须放在最外层
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (rs[i][k] != INT_MAX && rs[k][j] != INT_MAX &&   // 防止越界
            rs[i][j] > rs[i][k] + rs[k][j])
          rs[i][j] = rs[i][k] + rs[k][j];
      }
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix;
  AdjaMatrix(matrix, 5);
  SetMatrix(matrix, 0, 1, 2);
  SetMatrix(matrix, 0, 2, 5);
  SetMatrix(matrix, 0, 3, 1);
  SetMatrix(matrix, 1, 4, 3);
  SetMatrix(matrix, 2, 4, 4);
  SetMatrix(matrix, 3, 4, 2);
  SetMatrix(matrix, 1, 2, 4);
  SetMatrix(matrix, 2, 3, 3);
  std::vector<std::vector<int> > rs;
  Floyd(matrix, rs);
  LOG(INFO) << JoinMatrix(&rs);
  return 0;
}
