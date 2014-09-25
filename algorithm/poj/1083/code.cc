// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-27 00:24:53
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"
// TLE, 结果就是最大团的大小
namespace algorithm {
const int MAXN = 205;
int N;
struct Node {
  int b;
  int e;
} data[MAXN];

int matrix[MAXN][MAXN];
int dp[MAXN];
int stack[MAXN][MAXN];
int max_clique = 0;

bool Cmp(const Node & x, const Node & y) {
  return x.e < y.e;
}

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
      if (deep + N - u <= max_clique) return 0;
      if (deep + dp[u] <= max_clique) return 0;
      int cnt = 0;
      for (int j = i + 1; j < num; j++) {
        int v = stack[deep][j];
        if (matrix[u][v] > 0) stack[deep + 1][cnt++] = v;
      }
      if (DFS(deep + 1, cnt)) return 1;
    }
    return 0;
  }
}

void Build() {
  memset(matrix, 0, sizeof(matrix));
  std::sort(data, data + N, Cmp);
  for (int i = 0; i < N; i++) {
    for (int j = i - 1; j >= 0; j--) {
      if (data[j].e < data[i].b) break;
      matrix[j][i] = matrix[i][j] = 1;
    }
  }
}

int Dp() {
  max_clique = 0;
  memset(dp, 0, sizeof(dp));
  Build();
  for (int i = N - 1; i >= 0; i--) {
    int cnt = 0;
    for (int j = i + 1; j < N; j++) {
      if (matrix[i][j] > 0) stack[1][cnt++] = j;
    }
    DFS(1, cnt);
    dp[i] = max_clique;
  }
  return max_clique;
}


void Read() {
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int k = 0; k < c; k++) {
    memset(data, 0, sizeof(data));
    scanf("%d", &N);
    getchar();
    for (int i = 0; i < N; i++) {
      scanf("%d%d", &(data[i].b), &(data[i].e));
      if (data[i].b > data[i].e) std::swap(data[i].b, data[i].e);
      if (data[i].b % 2 == 0) data[i].b--;
      if (data[i].e % 2 == 1) data[i].e++;
      getchar();
    }
    int t = Dp();
    printf("%d\n", t * 10);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
