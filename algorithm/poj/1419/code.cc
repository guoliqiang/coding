// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-12 03:38:54
// File  : code.cc
// Brief :


#include "base/public/common_ojhead.h"

namespace algorithm {

/*
 * Accepted 176K  0MS C++
 *
 * 最大团　等于　反图的最大独立集合
 *
 * */

const int MAXN = 101;
int N = 0;
int result[MAXN];
int stack[MAXN][MAXN];
int matrix[MAXN][MAXN];
int dp[MAXN];
int path[MAXN];
int cnt = 0;

int DFS(int k, int n) {
  if (n == 0) {
    if (k > cnt) {
      cnt = k;
      memset(result, 0, sizeof(result));
      for (int i = 0; i < cnt; i++) {
        result[i] = path[i];
      }
      return 1;
    }
    return 0;
  } else {
    for (int i = 0; i < n; i++) {
      int u = stack[k][i];
      if (k + N - u <= cnt) return 0;
      if (k + dp[u] <= cnt) return 0;
      int num = 0;
      path[k] = u;
      for (int j = i + 1; j < n; j++) {
        int v = stack[k][j];
        if (matrix[u][v] > 0) stack[k + 1][num++] = v;
      }
      if (DFS(k + 1, num)) return 1;  // 没有必要等到ｉ循环完毕
    }
    return 0;
  }
}

void MaxClique() {
  cnt = 0;
  memset(result, 0, sizeof(result));
  memset(path, 0, sizeof(path));
  memset(dp, 0, sizeof(dp));
  for (int i = N - 1; i >= 0; i--) {
    path[0] = i;
    int num = 0;
    for (int j = i + 1; j < N; j++) {
      if (matrix[i][j] > 0) { 
        stack[1][num++] = j;
      }
    }
    DFS(1, num);
    dp[i] = cnt;
  }
}

void Read() {
  int m = 0;
  scanf("%d", &m);
  getchar();
  for (int i = 0; i < m; i++) {
    memset(matrix, 0, sizeof(matrix));
    int n = 0;
    int p = 0;
    scanf("%d%d", &n, &p);
    getchar();
    N = n;
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) matrix[j][k] = 1;
    }
    for (int j = 0; j < p; j++) {
      int u, v;
      scanf("%d%d", &u, &v);
      getchar();
      matrix[u - 1][v - 1] = 0;  // 反图
      matrix[v - 1][u - 1] = 0;  // 反图
    }
    MaxClique();
    printf("%d\n", cnt);
    for (int j = 0; j < cnt; j++) {
      printf("%d ", result[j] + 1);
    }
    printf("\n");
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
