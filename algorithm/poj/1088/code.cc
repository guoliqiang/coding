// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-23 22:27:13
// File  : code.cc
// Brief :

// Accepted 444K  32MS  C++

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 102;
int N;
int M;
struct Node {
  int v;
  int next;
} data[MAXN * MAXN * 4];
int head[MAXN * MAXN];
int deep[MAXN * MAXN];
int matrix[MAXN][MAXN];
int cnt;

void Add(int u, int v) {
  data[cnt].v = v;
  data[cnt].next = head[u];
  head[u] = cnt++;
}

void Build() {
  cnt = 0;
  memset(head, -1, sizeof(head));
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      if (j - 1 >= 0 && matrix[i][j - 1] < matrix[i][j]) Add(i * N + j, i * N + j - 1);
      if (j + 1 < N && matrix[i][j + 1] < matrix[i][j]) Add(i * N + j, i * N + j + 1);
      if (i - 1 >= 0 && matrix[i - 1][j] < matrix[i][j]) Add(i * N + j, (i - 1) * N + j);
      if (i + 1 < M && matrix[i + 1][j] < matrix[i][j]) Add(i * N + j, (i + 1) * N + j);
    }
  }
}

int Depth(int k) {
  if (deep[k] > 0) return deep[k];
  deep[k] = 1;
  for (int i = head[k]; i != -1; i = data[i].next) {
    int v = data[i].v;
    deep[k] = std::max(deep[k], Depth(v) + 1);
  }
  return deep[k];
}

int Solve() {
  Build();
  memset(deep, 0, sizeof(deep));
  int rs = 0;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      rs = std::max(rs, Depth(i * N + j));
    }
  }
  return rs;
}

void Read() {
  while (scanf("%d%d", &M, &N) != EOF) {
    getchar();
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < N; j++) {
        scanf("%d", &(matrix[i][j]));
      }
      getchar();
    }
    int t = Solve();
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
