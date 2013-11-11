// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-10 19:38:34
// File  : code.cc
// Brief :

/*
 * Accepted 252K  16MS
 *
 * 基于second MST 算法来判断unique MST
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 100;
int N = 0;
int matrix[MAXN][MAXN];
int flag[MAXN][MAXN];
int dp[MAXN][MAXN];
int pre[MAXN];
int dis[MAXN];
int visited[MAXN];

int Unique() {
  memset(visited, 0, sizeof(visited));
  memset(pre, -1, sizeof(pre));
  memset(flag, 0, sizeof(flag));
  memset(dp, 0, sizeof(dp));
  int rs = 0;

  for (int i = 0; i < N; i++) {
    if (matrix[0][i] > 0) {
      dis[i] = matrix[0][i];
      dp[0][i] = matrix[0][i];
      pre[i] = 0;
    } else {
      dis[i] = INF;
    }
  }
  dis[0] = 0;
  visited[0] = 1;
  
  for (int i = 1; i < N; i++) {
    int idx = -1;
    int min = INF;
    for (int j = 0; j < N; j++) {
      if (visited[j] == 0 && dis[j] < min) {
        min = dis[j];
        idx = j;
      }
    }
    if (idx == - 1) break;
    
    for (int j = 0; j < N; j++) {
      if (visited[j] == 1) {
        dp[j][idx] = dp[idx][j] = std::max(dp[j][pre[idx]], matrix[pre[idx]][idx]);
      }
    }
    flag[pre[idx]][idx] = 1;
    flag[idx][pre[idx]] = 1;
    visited[idx] = 1;
    rs += matrix[pre[idx]][idx];
    for (int j = 0; j < N; j++) {
      if (visited[j] == 0 && matrix[idx][j] > 0 && dis[j] > matrix[idx][j]) {
        dis[j] = matrix[idx][j];
        pre[j] = idx;
      }
    }
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0 && flag[i][j] == 0 && matrix[i][j] == dp[i][j]) {
        return -1;
      }
    }
  }
  return rs;
}

void Read() {
  int n = 0;
  int m = 0;
  int k = 0;
  scanf("%d", &k);
  getchar();
  for (int i = 0; i < k; i++) {
    memset(matrix, 0, sizeof(matrix));
    scanf("%d%d", &n, &m);
    getchar();
    N = n;
    for (int j = 0; j < m; j++) {
      int x, y, c;
      scanf("%d%d%d", &x, &y, &c);
      getchar();
      matrix[x - 1][y - 1] = matrix[y - 1][x - 1] = c;
    }
    int t = Unique();
    if (t == -1) printf("Not Unique!\n");
    else printf("%d\n", t);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
