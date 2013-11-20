// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-19 18:44:48
// File  : code.cc
// Brief :

/*
 * Accepted  440K 32MS
 * 输入是图的话，也是这个模板.
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 10009;
int N = 0;
struct Node {
  int v;
  int next;
} edge[MAXN];

int head[MAXN];
int visited[MAXN];
int root[MAXN];
int parent = 0;
bool flag = false;
int child1 = 0;
int child2 = 0;
int tail = 0;

void Add(int u, int v) {
  edge[tail].v = v;
  edge[tail].next = head[u];
  head[u] = tail++;
}

int Find(int k) {
  if (root[k] == k) return root[k];
  else return root[k] = Find(root[k]);
}

void Unit(int x, int y) {
  int px = Find(x);
  int py = Find(y);
  if (px == py) return;
  else root[px] = py;
}

void Tarjan(int k) {
  visited[k] = 1;
  for (int i = head[k]; i != -1; i = edge[i].next) {
    int v = edge[i].v;
    if (visited[v] == 0) Tarjan(v);
    Unit(v, k);
  }
  if (k == child1) {
    if (!flag) flag = true;
    else {
      parent = Find(child2);
    }
  }
  if (k == child2) {
    if (!flag) flag = true;
    else {
      parent = Find(child1);
    }
  }
}

void Tarjan() {
  memset(visited, 0, sizeof(visited));
  for (int i = 1; i <= N; i++) {
    root[i] = i;
  }
  flag = false;
  parent = -1;
  for (int i = 1; i <= N; i++) {
    if (visited[i] == 0) Tarjan(i);
  }
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
    memset(head, -1, sizeof(head));
    tail = 0;
    for (int i = 1; i < n; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      getchar();
      Add(u, v);
    }
    int ch1, ch2;
    scanf("%d%d", &ch1, &ch2);
    getchar();
    child1 = ch1;
    child2 = ch2;
    Tarjan();
    printf("%d\n", parent);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
