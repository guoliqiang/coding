// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-10 15:36:58
// File  : code2.cc
// Brief :

/*
Accepted  640K  313MS
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 510;
int N;
int M;

struct Node {
 int u;
 int v;
 int val;
} data[MAXN * MAXN];

int root[MAXN];

void Init() {
  for (int i = 0; i < N; i++) root[i] = i;
}

int Find(int k) {
  if (root[k] == k) return k;
  return root[k] = Find(root[k]);
}

bool Union(int x, int y) {
  int px = Find(x);
  int py = Find(y);
  if (px == py) return false;
  root[px] = py;
  return true;
}

bool Cmp(const Node & x, const Node & y) {
  return x.val < y.val;
}

int Kruskar() {
  Init();
  std::sort(data, data + M, Cmp);
  int rs = 0;
  for (int i = 0; i < M; i++) {
    if (Union(data[i].u, data[i].v)) {
      rs = std::max(rs, data[i].val);
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
    M = 0;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        int t = 0;
        scanf("%d", &t);
        if (i == j) continue;
        data[M].u = i;
        data[M].v = j;
        data[M++].val = t;
      }
      getchar();
    }
    int t = Kruskar();
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
