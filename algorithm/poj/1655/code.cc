// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-05 17:37:58
// File  : code.cc
// Brief :

/*
Accepted  576K  79MS
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 20100;
int N = 0;
struct Node {
  int v;
  int next;
} data[MAXN * 2];

int head[MAXN];
int rs_balance;
int rs_index;

// 返回ｒ下有多少个儿子节点
int Trace(int r, int father) {
  int max = 0;
  int son = 0;
  for (int i = head[r]; i != -1; i = data[i].next) {
    int v = data[i].v;
    if (v == father) continue;
    int t = Trace(v, r);
    son += t;
    max = std::max(max, t);
  }
  son += 1;
  max = std::max(max, N - son);
  if (max < rs_balance) {
    rs_balance = max;
    rs_index = r;
  }
  return son;
}

void Read() {
  int c, n;
  scanf("%d", &c);
  getchar();
  for (int k = 0; k < c; k++) {
    rs_balance = INF;
    rs_index = -1;
    memset(head, -1, sizeof(head));
    scanf("%d", &n);
    getchar();
    N = n;
    for (int i = 0; i < n - 1; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      getchar();
      data[i * 2].v = v;
      data[i * 2].next = head[u];
      head[u] = i * 2;
      data[i * 2 + 1].v = u;
      data[i * 2 + 1].next = head[v];
      head[v] = i * 2 + 1;
    }
    Trace(1, 0);
    printf("%d %d\n", rs_index, rs_balance);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
