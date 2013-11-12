// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-11 23:38:29
// File  : code.cc
// Brief :
/*
 *  弱连通图判断
 *  Accepted  8016K 1125MS  C++
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1001;
int N = 0;

int matrix[MAXN][MAXN];
int tmatrix[MAXN][MAXN];
int low[MAXN];
int dfsn[MAXN];
int id[MAXN];
int stack[MAXN];
int in[MAXN];
int tail = 0;
int num = 0;
int tag = 0;

void Tarjan(int k) {
  dfsn[k] = low[k] = num++;
  stack[tail++] = k;
  int min = N;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (dfsn[i] == -1) Tarjan(i);
      min = std::min(low[i], min);
    }
  }
  if (min < low[k]) {
    low[k] = min;
  } else {
    tail -= 1;
    while (stack[tail] != k) {
      id[stack[tail]] = tag;
      low[stack[tail]] = N;
      tail--;
    }
    id[stack[tail]] = tag++;
    low[stack[tail]] = N;
  }
}

bool DFS(int k, int deep) {
  if (deep == tag) return true;
  for (int i = 0; i < tag; i++) {
    if (tmatrix[k][i] > 0) {
      if (DFS(i, deep + 1)) return true;
    }
  }
  return false;
}

void Judge() {
  memset(dfsn, -1, sizeof(dfsn));
  memset(low, -1, sizeof(low));
  memset(stack, -1, sizeof(stack));
  tail = 0;
  num = 0;
  tag = 0;
  for (int i = 0; i < N; i++) {
    if (dfsn[i] == -1) Tarjan(i);
  }
  memset(in, 0, sizeof(in));
  memset(tmatrix, 0, sizeof(tmatrix));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0 && id[i] != id[j]) {
        in[id[j]]++;
        tmatrix[id[i]][id[j]] = 1;
      }
    }
  }
  int rs = 0;
  int idx = 0;
  for (int i = 0; i < tag; i++) {
    if (in[i] == 0) {
      rs++;
      idx = i;
    }
  }
  if (rs != 1) printf("No\n");
  else {
    if (DFS(idx, 1)) printf("Yes\n");
    else printf("No\n");
  }
}

void Read() {
  int T = 0;
  scanf("%d", &T);
  getchar();
  for (int k = 0; k < T; k++) {
    memset(matrix, 0, sizeof(matrix));
    int n = 0;
    int m = 0;
    scanf("%d%d", &n, &m);
    getchar();
    N = n;
    for (int i = 0; i < m; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      getchar();
      matrix[u - 1][v - 1] = 1;
    }
    Judge();
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
