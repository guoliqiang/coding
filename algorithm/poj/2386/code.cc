// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just Want to GH to hss~
// Date  : 2014-01-26 11:34:23
// File  : code.cc
// Brief :

// Accepted 604K  0MS

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 102;
int N;
int M;
int matrix[MAXN][MAXN];
char data[MAXN][MAXN];

void DFS(int x, int y, int v) {
  matrix[x][y] = v;
  if (x + 1 < M && data[x + 1][y] == 'W' && matrix[x + 1][y] == 0) DFS(x + 1, y, v);
  if (x + 1 < M && y - 1 >= 0 && data[x + 1][y - 1] == 'W' && matrix[x + 1][y - 1] == 0) DFS(x + 1, y - 1, v);
  if (x + 1 < M && y + 1 < N && data[x + 1][y + 1] == 'W' && matrix[x + 1][y + 1] == 0) DFS(x + 1, y + 1, v);

  if (y - 1 >= 0 && data[x][y - 1] == 'W' && matrix[x][y - 1] == 0) DFS(x, y - 1, v);
  if (y + 1 < N && data[x][y + 1] == 'W' && matrix[x][y + 1] == 0) DFS(x, y + 1, v);

  if (x - 1 >= 0 && data[x - 1][y] == 'W' && matrix[x - 1][y] == 0) DFS(x - 1, y, v);
  if (x - 1 >= 0 && y - 1 >= 0 && data[x - 1][y - 1] == 'W' && matrix[x - 1][y - 1] == 0) DFS(x - 1, y - 1, v);
  if (x - 1 >= 0 && y + 1 < N && data[x - 1][y + 1] == 'W' && matrix[x - 1][y + 1] == 0) DFS(x - 1, y + 1, v);
}

int Solve() {
  memset(matrix, 0, sizeof(matrix));
  int cnt = 1;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      if (data[i][j] == 'W' && matrix[i][j] == 0) DFS(i, j, cnt++);
    }
  }
  return cnt - 1;
}

void Read() {
  char ch[MAXN];
  scanf("%d%d", &M, &N);
  getchar();
  for (int i = 0; i < M; i++) {
    scanf("%s", ch);
    for (int j = 0; j < N; j++) {
      data[i][j] = ch[j];
    }
  }
  int t = Solve();
  printf("%d\n", t);
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
