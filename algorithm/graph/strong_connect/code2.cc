// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-24 20:16:48
// File  : code2.cc
// Brief :

// tarjan强联通分量

#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int dfn[MAX];
int low[MAX] = {0};
int id[MAX] = {0};
int stack[MAX] = {0};
int stop = 0;

int component_tag = 0;
int cnt = 1;

// 无向图双联通分量
// low[]的编号即为连通分量的编号
// poj3352
// 双连通图：在无向图连通图中，如果删除该图中的任意一点和依附它的边，不改变图的连通性，则称该图为双连通的无向图。
//           由上述定义可知，双连通分量中，每两个结点之间至少有两条不同的路径可以相互到达。

// 无向图联通分量（不同于双联通分量, 直接DFS即可，从一个点dfs到的点都处于同一个联通分量中）
// http://en.wikipedia.org/wiki/Connected_component_%28graph_theory%29
void Tarjan(int r, int father) {
  dfn[r] = low[r] = cnt++;
  for (int i = 0; i < N; i++) {
    if (matrix[r][i] <= 0 || i == father) continue;
    if (dfn[i] == -1) Tarjan(i, r);
    low[r] = std::min(low[i], low[r]);
  }
}

// 有向图的极大强联通分量
void Tarjan(int k) {
  dfn[k] = low[k] = cnt++;
  stack[stop++] = k;
  int min = low[k];
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (dfn[i] == -1) Tarjan(i);
      min = std::min(min, low[i]);
    }
  }
  if (low[k] > min) {
    low[k] = min;
  } else {
    LOG(INFO) << "strong component " << component_tag << ":";
    stop--;
    while (stack[stop] != k) {
      LOG(INFO) << stack[stop];
      id[stack[stop]] = component_tag;
      low[stack[stop]] = N;  // trick
      stop--;
    }
    LOG(INFO) << k;
    id[k] = component_tag;
    low[k] = N;
    component_tag++;
  }
}

void Read(std::vector<std::vector<int> > & v) {
  N = v.size();
  for (int i = 0; i < N; i++) dfn[i] = -1;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) matrix[i][j] = v[i][j];
  }
}

void Find() {
  for (int i = 0; i < N; i++) {
    if (dfn[i] == -1) {
      Tarjan(i);  // 有向图
      // Tarjan(i, -1)  // 无向图
    }
  }
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 6;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][2] = 1;
  v[0][1] = 1;
  v[1][3] = 1;
  v[2][3] = 1;
  v[3][0] = 1;
  v[2][4] = 1;
  v[3][5] = 1;
  v[4][5] = 1;
  Read(v);
  Find();
  return 0;
}
