// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-22 19:16:28
// File  : code.cc
// Brief : http://www.cnblogs.com/zhj5chengfeng/archive/2013/07/29/3224092.html
// 无向图的最大独立数: 从V个顶点中选出k个顶，使得这k个顶互不相邻。 那么最大的k就是这个图的最大独立数.
// 无向图的最大团:     从V个顶点选出k个顶，使得这k个顶构成一个完全图，即该子图任意两个顶都有直接的边.

// 1、最大团点的数量=补图中最大独立集点的数量  poj1419
// 2、二分图中，最大独立集点的数量+最小覆盖点的数量=整个图点的数量  // 显然
// 3、二分图中，最小覆盖点的数量=最大匹配的数量
// 4、图的染色问题中，最少需要的颜色的数量=最大团点的数量

#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int stack[MAX][MAX] = {{0}};
int max_clique = 0;
int dp[MAX] = {0};  // dp[i] 表示 包含点[i, n-1]的最大团的点的数目


int DFS(int deep, int num) {
  if (num == 0) {
    if (deep > max_clique) {
      max_clique = deep;
      return 1;
    }
    return 0;
  } else {
    for (int i = 0; i < num; i++) {
      int u = stack[deep][i];
      if (deep + N - u <= max_clique) return 0;  // 剪枝1
      if (deep + dp[u] <= max_clique) return 0;  // 剪枝2
      int cnt = 0;
      for (int j = i + 1; j < num; j++) {
        int v = stack[deep][j];
        if (matrix[u][v] > 0) stack[deep + 1][cnt++] = v;
      }
      if (DFS(deep + 1, cnt)) return 1;  // 剪枝3
      // 当返回1时，i之后的点不可能再扩展出一个比当前团还要大的团出来
    }
    return 0;
  }
}

int MaxClique(std::vector<std::vector<int> > & v) {
  N = v.size();
  memset(dp, 0, sizeof(dp));
  memset(stack, 0, sizeof(stack));
  max_clique = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
    }
  }

  for (int i = N - 1; i >= 0; i--) {
    int cnt = 0;
    for (int j = i + 1; j < N; j++) {
      if (matrix[i][j] > 0) stack[1][cnt++] = j;
    }
    DFS(1, cnt); // Note begin from 1, 0 其实是被ｉ占据了
    dp[i] = max_clique;
  }
  return max_clique;
}

}  // namespace algorithm

using namespace algorithm;

namespace NB {
}


int main(int argc, char** argv) {
  int n = 6;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][2] = v[2][0] = 10;
  v[1][0] = v[0][1] = 3;
  v[1][2] = v[2][1] = 3;
  v[1][3] = v[3][1] = 7;
  v[2][4] = v[4][2] = 8;
  v[3][2] = v[2][3] = 3;
  v[3][5] = v[5][3] = 5;
  v[4][1] = v[1][4] = 8;
  v[5][4] = v[4][5] = 6;
  LOG(INFO) << MaxClique(v);
  return 0;
}
