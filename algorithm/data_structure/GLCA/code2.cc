// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-04 02:27:20
// File  : code2.cc
// Brief :

/*
 * tarjan
 * offline离线算法
 * 需要提前给出要查询的query, 或把所有的对都求出来。
 *
 * 处理：O(E)
 * 查询：O(1)
 * 空间复杂度： O(n^2)
 *
 * 
 * poj1330
 * 感觉tarjan更好写，比rmq更直接,rmq可以使用稀疏矩阵节约内存
 *
 * */
#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int visited[MAX] = {0};
int lcs[MAX][MAX] = {{0}};
int root[MAX] = {0};

int iroot(int k) {
  if (root[k] == k) return k;
  return root[k] = iroot(root[k]);
}

void unit(int x, int y) {
  int tx = iroot(x);
  int ty = iroot(y);
  if (tx == ty) return;
  root[tx] = ty;
}

void DFS(int k) {
  visited[k] = 1;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] <= 0) continue;
    if (visited[i] == 0) {
      DFS(i);
      unit(i, k);
    }
  }
  for (int i = 0; i < N; i++) {
    if (visited[i] == 1) {
      lcs[k][i] = lcs[i][k] = iroot(i);
      // LOG(INFO) << "lcs[" << k << "][" << i << "]:" << lcs[k][i] << " lcs[" << i << "][" << k << "]:" << lcs[i][k];
    }
  }
}

void DFS() {
  memset(root, 0xff, sizeof(root));
  for (int i = 0; i < N; i++) {
    root[i] = i;
  }
  memset(visited, 0, sizeof(visited));
  memset(lcs, 0xff, sizeof(lcs));
  DFS(0);  // 如果是有向图的话，输入要是一个入度为0的节点
  // 如果图不保证连通
  // for (int i = 0; i < N; i++) {
  //   if (visited[i] == 0) DFS(i);
  // }
}

int Get(int x, int y) {
  return lcs[x][y];
}

void Read(std::vector<std::vector<int> > & v) {
  N = v.size();
  memset(matrix, 0, sizeof(matrix));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
    }
  }
  DFS();
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int  n = 6;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][1] = v[1][0] = 2;
  v[1][3] = v[3][1] = 2;
  v[1][2] = v[2][1] = 2;
  v[0][2] = v[2][0] = 2;
  v[1][4] = v[4][1] = 2;
  v[2][5] = v[5][2] = 2;
  Read(v);
  LOG(INFO) << Get(3, 4);
 
  return 0;
}
