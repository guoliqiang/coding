// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-12 19:48:31
// File  : code.cc
// Brief :

/*
 * Accepted 252K  32MS
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 101;
int N = 0;
int matrix[MAXN][MAXN];
int dis[MAXN][MAXN];
int path[MAXN][MAXN];
int cnt = -1;
int len = INF;
int result[MAXN];

bool  MiniLoop() {
  memset(dis, 0, sizeof(dis));
  memset(path, -1, sizeof(path));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j) {
        dis[i][j] = 0;
      } else if (matrix[i][j] > 0) {
        dis[i][j] = matrix[i][j];
        path[i][j] = i;
      } else {
        dis[i][j] = INF;
      }
    }
  }
  for (int k = 0; k < N; k++) {
    for (int i = 0; i < k; i++) {
      for (int j = i + 1; j < k; j++) {
        if (matrix[j][k] > 0 && matrix[k][i] > 0 && dis[i][j] + matrix[j][k] + matrix[k][i] < len) {
          len = dis[i][j] + matrix[j][k] + matrix[k][i];
          // LOG(INFO) << "len:" << len;
          cnt = 0;
          result[cnt++] = k;
          int t = j;
          while (t != i) {
            // LOG(INFO) << "path[" << i << "][" << t << "]:" << path[i][t];
            result[cnt++] = t;
            t = path[i][t];
          }
          result[cnt++] = i;
        }
      }
    }

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (dis[i][j] > dis[i][k] + dis[k][j]) {
          dis[i][j] = dis[i][k] + dis[k][j];
          path[i][j] = path[k][j];
        }
      }
    }
  }
  return cnt == -1 ? false : true;
}

void Read() {
  int n, m;
  while (scanf("%d%d", &n, &m) != EOF) {
    getchar();
    N = n;
    memset(matrix, 0, sizeof(matrix));
    for (int i = 0; i < m; i++) {
      int u, v, c;
      scanf("%d%d%d", &u, &v, &c);
      getchar();
      if (matrix[u - 1][v - 1] == 0 || matrix[u - 1][v - 1] > c) {
        matrix[u - 1][v - 1] = c;
        matrix[v - 1][u - 1] = c;
      }
      // LOG(INFO) << "matrix[" << u - 1 << "][" << v - 1 << "]:" << matrix[u - 1][v - 1];
    }
  }
  if (!MiniLoop()) printf("No solution.\n");
  else {
    for (int i = cnt - 1; i >= 0; i--) {
      printf("%d ", result[i] + 1);
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
