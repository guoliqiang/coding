// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-21 12:07:57
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 510;
int N;
const int MAXM = 2800 * 2;
int head[MAXN];
struct Node {
  int v;
  int d;
  int next;
} e[MAXM];
int stop;

int dis[MAXN];
int flag[MAXN];
int cnt[MAXN];

void Add(int u, int v, int dis) {
  e[++stop].v = v;
  e[stop].d = dis;
  e[stop].next = head[u];
  head[u] = stop;
}

bool SPFA(int s) {
  memset(flag, 0, sizeof(flag));
  memset(cnt, 0, sizeof(cnt));
  for (int i = 1; i <= N; i++) dis[i] = INF;
  std::queue<int> queue;
  queue.push(s);
  dis[s] = 0;
  flag[s] = 1;
  cnt[s]++;
  while (!queue.empty()) {
    int t = queue.front();
    queue.pop();
    flag[t] = 0;
    for (int i = head[t]; i != -1; i = e[i].next) {
      int v = e[i].v;
      if (dis[v] > dis[t] + e[i].d) {
        dis[v] = dis[t] + e[i].d;
        if (flag[v] == 0) {
          flag[v] = 1;
          cnt[v]++;
          if (cnt[v] > N) return true;
          queue.push(v);
        }
      }
    }
  }
  return false;
}

void Read() {
  int T;
  scnaf("%d", &T);
  getchar();
  for (int k = 0; k < T; k++) {
    int n, m, w;
    scnaf("%d%d%d", &n, &m, &w);
    getchar();
    N = n;
    stop = 0
    memset(head, -1, sizeof(head));
    for (int i = 0; i < m; i++) {
      int u, v, d;
      scnaf("%d%d%d", &u, &v, &d);
      getchar();
      Add(u, v, d);
      Add(v, u, d);
    }
    for (int i = 0; i < w; i++) {
      int u, v, d;
      scnaf("%d%d%d", &u, &v, &d);
      getchar();
      Add(u, v, -d);
    }
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
