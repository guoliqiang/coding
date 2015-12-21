// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-17 18:08:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1000;
int N;
int matrix[MAXN][MAXN];
int top[MAXN];
int indegree[MAXN];
int vb[MAXN];    // 事件最早开始的时间
int ve[MAXN];    // 事件最晚开始的时间
int path[MAXN];  // 关键路径


bool TopOrder() {
  memset(indegree, 0, sizeof(indegree));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0) indegree[j]++;
    }
  }
  int tail = 0;
  for (int i = 0; i < N; i++) {
    if (indegree[i] == 0) top[tail++] = i;
  }
  int head = 0;
  while (head < tail) {
    int t = top[head++];
    for (int i = 0; i < N; i++) {
      if (matrix[t][i] > 0) {
        indegree[i]--;
        if (indegree[i] == 0) top[tail++] = i;
      }
    }
  }
  return tail == N;
}

bool Aoe() {
  if (!TopOrder()) return false;
  memset(vb, 0, sizeof(vb));
  vb[top[0]] = 0;
  for (int i = 0; i < N; i++) {
    int t = top[i];
    for (int j = 0; j < N; j++) {
      if (matrix[t][j] > 0) vb[j] = std::max(vb[j], vb[t] + matrix[t][j]);
    }
  }
  for (int i = 0; i < N; i++) ve[i] = INF;
  ve[top[N - 1]] = vb[top[N - 1]];
  for (int i = N - 1; i >= 0; i--) {
    int t = top[i];
    for (int j = 0; j < N; j++) {
      if (matrix[j][t] > 0) ve[j] = std::min(ve[j], ve[t] - matrix[j][t]);
    }
  }
  memset(path, 0, sizeof(path));
  for (int i = 0; i < N; i++) {
    if (vb[i] == ve[i]) path[i] = 1;
  }
  return true;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
