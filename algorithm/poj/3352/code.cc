// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-13 17:18:26
// File  : code.cc
// Brief :

/*
 * Accepted 4096K 47MS
 * 
 * tarjan深度优先搜索
 *
 * low[id] 相同的点处于同一个强(双向)连通分量
 * 
 * 
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1002;
int N = 0;
int matrix[MAXN][MAXN];
int dfsn[MAXN];
int low[MAXN];
int degree[MAXN];
int num = 0;
int cnt = 0;

void DFS(int k, int father) {
  dfsn[k] = low[k] = num++;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (dfsn[i] == -1) {
        DFS(i, k);
        low[k] = std::min(low[k], low[i]);
        if (low[i] > dfsn[k]) {
          // LOG(INFO) << k << "~" << i;
          cnt++;
        }
      } else {
        if (i != father) low[k] = std::min(low[k], dfsn[i]);
      }
    }
  }
}

int DFS() {
  num = 0;
  cnt = 0;
  memset(dfsn, -1, sizeof(dfsn));
  memset(low, -1, sizeof(low));
  memset(degree, 0, sizeof(degree));
  DFS(0, -1);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0 && low[i] != low[j]) {
        degree[low[i]]++;
      }
    }
  }
  int rs = 0;
  for (int i = 0; i < N; i++) {
    if (degree[i] == 1) rs++;
  }
  return (rs + 1) / 2;
}


void Read() {
  int n, r;
  while (scanf("%d%d", &n, &r) != EOF) {
    getchar();
    memset(matrix, 0, sizeof(matrix));
    N = n;
    for (int i = 0; i < r; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      getchar();
      matrix[u - 1][v - 1] = 1;
      matrix[v - 1][u - 1] = 1;
      // LOG(INFO) << "matrix[" << u - 1 << "][" << v - 1 << "]:" << matrix[u - 1][v - 1];
      // LOG(INFO) << "matrix[" << v - 1 << "][" << u - 1 << "]:" << matrix[v - 1][u - 1];
    }
    int t = DFS();
    printf("%d\n", t);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
