// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-24 11:53:41
// File  : code.cc
// Brief :

// Accepted 228K  0MS
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 110;
int N;
int matrix[MAXN][MAXN];
int degree[MAXN];
int dis[MAXN][MAXN];

bool Check() {
  int t = 0;
  for (int i = 0; i < N; i++) {
    if (degree[i] == 0) {
      t++;
      if (t > 1) return false;
    }
  }
  return true;
}

void Floyd() {
  memset(dis, 0, sizeof(dis));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j) dis[i][j] = 0;
      else if (matrix[i][j] > 0) dis[i][j] = matrix[i][j];
      else dis[i][j] = INF;
    }
  }
  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (dis[i][j] > dis[i][k] + dis[k][j]) dis[i][j] = dis[i][k] + dis[k][j];
      }
    }
  }
}

void Solve() {
  if (Check() == false) {
    printf("disjoint\n");
    return;
  }
  Floyd();
  int idx = -1;
  int min = INF;

  for (int i = 0; i < N; i++) {
    int tmax = 0;
    for (int j = 0; j < N; j++) tmax = std::max(tmax, dis[i][j]);
    if (tmax < min) {
      min = tmax;
      idx = i;
    }
  }
  printf("%d %d\n", idx + 1, min);
}

void Read() {
  while (scanf("%d", &N) != EOF && N != 0) {
    getchar();
    memset(degree, 0, sizeof(degree));
    memset(matrix, 0, sizeof(matrix));
    for (int i = 0; i < N; i++) {
      int cnt = 0;
      scanf("%d", &cnt);
      int v;
      int val;
      for (int j = 0; j < cnt; j++) {
        scanf("%d%d", &v, &val);
        v -= 1;
        matrix[i][v] = val;
        degree[v]++;
      }
      getchar();
    }
    Solve();
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}

