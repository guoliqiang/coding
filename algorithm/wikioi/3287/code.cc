// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-20 21:28:33
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 10003;
int N;
const int MAXM = 50005;
int M;
struct Node {
  int u;
  int v;
  int val;
  int next;
} e1[MAXM], e2[MAXM * 2];
int stop = 0;
int f[MAXN][14], g[MAXN][14];

int head[MAXN];
int root[MAXN];
int level[MAXN];

bool Cmp(const Node & x, const Node & y) {
  return x.val > y.val;
}

int Find(int k) {
  if (root[k] == k) return k;
  return root[k] = Find(root[k]);
}

void Unit(int x, int y) {
  int px = Find(x);
  int py = Find(y);
  if (px == py) return;
  root[px] = py;
}

void Add(int u, int v, int val) {
  e2[++stop].u = u; e2[stop].v = v; e2[stop].val = val; e2[stop].next = head[u]; head[u] = stop;
  e2[++stop].u = v; e2[stop].v = u; e2[stop].val = val; e2[stop].next = head[v]; head[v] = stop;
}

// 最大生成树算法
void MaxST() {
  stop = 0;
  memset(head, -1, sizeof(head));
  for (int i = 0; i <= N; i++) root[i] = i;
  std::sort(e1, e1 + M, Cmp);
  for (int i = 0; i < M; i++) {
    int pu = Find(e1[i].u);
    int pv = Find(e1[i].v);
    if (pu != pv) {
      Unit(pu, pv);
      Add(e1[i].u, e1[i].v, e1[i].val);
    }
  }
}

void DFS(int r, int lev) {
  level[r] = lev;
  for (int i = 1; i <= 13; i++) {
    f[r][i] = f[f[r][i - 1]][i - 1];
    g[r][i] = std::min(g[f[r][i - 1]][i - 1], g[r][i - 1]);
  }
  for (int i = head[r]; i != -1; i = e2[i].next) {
    int v = e2[i].v;
    if (level[v] == 0) {
      f[v][0] = r;
      g[v][0] = e2[i].val;
      DFS(v, lev + 1);
    }
  }
}

void Build() {
  memset(level, 0, sizeof(level));
  memset(f, 0, sizeof(f));
  memset(g, 0, sizeof(g));
  for (int i = 1; i <= N; i++) {
    if (level[i] == 0) DFS(i, 1);
  }
}

// 对于点很多的情况这种LCA效率很高
int LCA(int x, int y) {
  int rs = INF;
  if (level[x] > level[y]) std::swap(x, y);
  // 因为任何整数都可以表示成二进制形式
  // 所以for循环可以表示出level相差任何数的情况
  for (int i = 13; i >= 0; i--) {
    if (level[f[y][i]] >= level[x]) {
      rs = std::min(rs, g[y][i]);
      y = f[y][i];
    }
  }
  if (y == x) return rs;
  for (int i = 13; i >= 0; i--) {
    if (f[y][i] != f[x][i]) {
      rs = std::min(rs, g[y][i]);
      rs = std::min(rs, g[x][i]);
      y = f[y][i];
      x = f[x][i];
    }
  }
  rs = std::min(rs, g[y][0]);
  rs = std::min(rs, g[x][0]);
  return rs;
}

void Read() {
  while (scanf("%d%d", &N, &M) != EOF) {
    getchar();
    for (int i = 0; i < M; i++) {
      int u, v, t;
      scanf("%d%d%d", &u, &v, &t);
      getchar();
      e1[i].u = u;
      e1[i].v = v;
      e1[i].val = t;
    }
    MaxST();
    Build();
    int q = 0;
    scanf("%d", &q);
    getchar();
    for (int i = 0; i < q; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      getchar();
      int t = -1;
      if (Find(u) == Find(v)) t = LCA(u, v);
      printf("%d\n", t);
    }
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
