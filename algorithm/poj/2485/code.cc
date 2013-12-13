// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-10 14:52:48
// File  : code.cc
// Brief :

/*
Accepted  568K  157MS
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 510;
int N;
int matrix[MAXN][MAXN];
int visited[MAXN];
int dis[MAXN];

int Prime(int source) {
  int rs = 0;
  memset(visited, 0, sizeof(visited));
  for (int i = 0; i < N; i++) dis[i] = INF;
  dis[source] = 0;
  for (int k = 0; k < N; k++) {
    int min = INF;
    int index = -1;
    for (int i = 0; i < N; i++) {
      if (visited[i] == 0 && dis[i] < min) {
        min = dis[i];
        index = i;
      }
    }
    if (index == -1) break;
    visited[index] = 1;
    rs = std::max(rs, dis[index]);
    for (int i = 0; i < N; i++) {
      if (visited[i] == 0 && matrix[index][i] > 0 && dis[i] > matrix[index][i]) {
        dis[i] = matrix[index][i];
      }
    }
  }
  return rs;
}

void Read() {
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int k = 0; k < c; k++) {
    int n = 0;
    scanf("%d", &n);
    getchar();
    N = n;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        int t = 0;
        scanf("%d", &t);
        matrix[i][j] = t;
      }
      getchar();
    }
    int t = Prime(0);
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
