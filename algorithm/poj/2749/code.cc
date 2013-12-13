// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-10 17:00:08
// File  : code.cc
// Brief :

/*
Accepted  4160K 1860MS

二分找距离，NB
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 502;
int N;
int A;
int B;

int matrix[MAXN * 2][MAXN * 2];
int dis[MAXN * 2];
struct Node {
  int u;
  int v;
} friends[MAXN * 2], hates[MAXN * 2];
Node s[2];

int dfsn[MAXN * 2];
int stack[MAXN * 2];
int low[MAXN * 2];
int tag[MAXN * 2];
int num;
int tagnum;
int stop;

void Tarjan(int r) {
  dfsn[r] = low[r] = num++;
  stack[stop++] = r;
  int min = N * 2;
  for (int i = 0; i < N * 2; i++) {
    if (matrix[r][i] > 0) {
      if (dfsn[i] == -1) Tarjan(i);
      min = std::min(min, low[i]);
    }
  }
  if (low[r] > min) {
    low[r] = min;
  } else {
    stop--;
    while (stack[stop] != r) {
      tag[stack[stop]] = tagnum;
      // LOG(INFO) << "tag[" << stack[stop] << "]:" << tagnum;
      low[stack[stop]] = 2 * N;
      stop--;
    }
    tag[stack[stop]] = tagnum++;
    low[stack[stop]] = 2 * N;
  }
}

void Tarjan() {
  memset(dfsn, -1, sizeof(dfsn));
  memset(stack, 0, sizeof(stack));
  memset(low, -1, sizeof(low));
  num = 0;
  tagnum = 0;
  stop = 0;
  for (int i = 0; i < N * 2; i++) {
    if (dfsn[i] == -1) Tarjan(i);
  }
}

bool Check() {
  for (int i = 0; i < N; i++) {
    if (tag[i] == tag[N + i]) return false;
  }
  return true;
}


void Build(int mid) {
  int d = abs(s[0].u - s[1].u) + abs(s[0].v - s[1].v);
  memset(matrix, 0, sizeof(matrix));
  for (int i = 0; i < B; i++) {
    int u = friends[i].u;
    int v = friends[i].v;
    matrix[u][v] = matrix[v][u] = 1;
    matrix[N + u][N + v] = matrix[N + v][N + u] = 1;
  }
  for (int i = 0; i < A; i++) {
    int u = hates[i].u;
    int v = hates[i].v;
    matrix[N + u][v] = matrix[v][N + u] = 1;
    matrix[u][N + v] = matrix[N + v][u] = 1;
  }
  for (int i = 0; i < N; i++) {
    for (int j = i + 1; j < N; j++) {
      if (dis[i] + dis[j] > mid) {
        matrix[i][N + j] = 1;
        matrix[j][N + i] = 1;
      }
      if (dis[i + N] + dis[j + N] > mid) {
        matrix[i + N][j] = 1;
        matrix[j + N][i] = 1;
      }
      if (dis[i] + dis[j + N] + d > mid) {
        matrix[i][j] = 1;
        matrix[j + N][i + N] = 1;
      }
      if (dis[j] + dis[i + N] + d > mid) {
        matrix[j][i] = 1;
        matrix[i + N][j + N] = 1;
      }
    }
  }
}

void Solve() {
  int b = 0;
  int e = INF;
  int rs = -1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    Build(mid);
    Tarjan();
    if (Check()) {
      rs = (rs == -1) ? mid : std::min(rs, mid);
      e = mid - 1;
    } else {
      b = mid + 1;
    }
  }
  printf("%d\n", rs);
}

void Read() {
  int n, a, b;
  while(scanf("%d%d%d", &n, &a, &b) != EOF) {
    getchar();
    N = n;
    A = a;
    B = b;
    memset(dis, 0, sizeof(dis));
    for (int i = 0; i < 2; i++) {
      int x, y;
      scanf("%d%d", &x, &y);
      s[i].u = x;
      s[i].v = y;
    }
    getchar();
    for (int i = 0; i < n; i++) {
      int x, y;
      scanf("%d%d", &x, &y);
      getchar();
      dis[i] = abs(x - s[0].u) + abs(y - s[0].v);
      dis[N + i] = abs(x - s[1].u) + abs(y - s[1].v);
    }
    memset(friends, 0, sizeof(friends));
    memset(hates, 0, sizeof(hates));
    for (int i = 0; i < a; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      getchar();
      hates[i].u = u - 1;
      hates[i].v = v - 1;
    }
    for (int i = 0; i < b; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      getchar();
      friends[i].u = u - 1;
      friends[i].v = v - 1;
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
