// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-09 22:04:17
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 55;
int matrix[MAXN][MAXN];
int vis[MAXN][MAXN];
int M;
int N;
int bridge = 0;
int city = 0;


void DFS(int x, int y) {
  vis[x][y] = 1;
  if (x + 1 < M && matrix[x + 1][y] == 1 && vis[x + 1][y] == 0) DFS(x + 1, y);
  if (x - 1 >= 0 && matrix[x - 1][y] == 1 && vis[x - 1][y] == 0) DFS(x - 1, y);
  if (y + 1 < N && matrix[x][y + 1] == 1 && vis[x][y + 1] == 0) DFS(x, y + 1);
  if (y - 1 >= 0 && matrix[x][y - 1] == 1 && vis[x][y - 1] == 0) DFS(x, y - 1);
  for (int i = -2; i <= 2; i += 4) {
    int tx, ty;
    tx = x + i;
    if (tx < 0 || tx >= M) continue;
    for (int j = -1; j <= 1; j++) {
      ty = y + j;
      if (ty < 0 || ty >= N) continue;
      if (matrix[tx][ty] == 1 && vis[tx][ty] == 0) {
        bridge++;
        DFS(tx, ty);
      }
    }
  }
  for (int i = -2; i <= 2; i += 4) {
    int tx, ty;
    ty = y + i;
    if (ty < 0 || ty >= N) continue;
    for (int j = -1; j <= 1; j++) {
      tx = x + j;
      if (tx < 0 || tx >= M) continue;
      if (matrix[tx][ty] == 1 && vis[tx][ty] == 0) {
        bridge++;
        DFS(tx, ty);
      }
    }
  }
}

void DFS() {
  bridge = 0;
  city = 0;
  memset(vis, 0, sizeof(vis));
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] == 1 && vis[i][j] == 0) {
        int pre = bridge;
        DFS(i, j);
        city += bridge - pre + 1;
      }
    }
  }
}

void Read() {
  while (scanf("%d%d", &M, &N) != EOF) {
    getchar();
    char ch;
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < N; j++) {
        scanf("%c", &ch);
        if (ch == '#') matrix[i][j] = 1;
        else matrix[i][j] = 0;
      }
      getchar();
    }
    DFS();
    printf("%d\n%d %d\n", city, bridge, bridge);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
