// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-07 00:55:13
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

#define M 109
#define type double
const type inf = 1<<30;
struct point  {
  double x;
  double y;
} p[M];

double distance(const point &a, const point &b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

struct Node {
  int u;
  int v;
  type cost;
} E[M*M];

int to[M];
int ID[M];
int vis[M];
type dis[M];
int n;
int m;

// 缩点模板
type Directed_MST(int root, int NV, int NE) {
  type ret = 0;
  while (true) {
    for (int i = 0; i < NV; i++) dis[i] = inf;
    for (int i = 0; i < NE; i++) {
      int u = E[i].u;
      int v = E[i].v;
      if (u != v && E[i].cost < dis[v]) {
        to[v] = u;
        dis[v] = E[i].cost;
      }
    }
    dis[root] = 0;
    for (int i = 0;i < NV;i++) {
      if (i == root) continue;
      if (dis[i] == inf)  return -1;
    }
    // 求环时要求除根节点以外的节点形成的环
    int cntnode = 0;
    memset(ID, -1, sizeof(ID));
    memset(vis, -1, sizeof(vis));
    for (int i = 0;i < NV; i++) {
      ret += dis[i];
      int v = i;
      while (v != root && vis[v] == -1) {
        vis[v] = i;
        v = to[v];
      }
      if (v != root && vis[v] == i) {  //  两个条件必须都有
        for (int u = to[v]; u != v; u = to[u]) {
          ID[u] = cntnode;
        }
        ID[v] = cntnode++;
      }
    }
    if (cntnode == 0)  break;
    for (int i = 0; i < NV; i++)  {
      if (ID[i] == -1) ID[i] = cntnode++;
    }
    //  缩点,重新标记
    for (int i = 0;i < NE; i++) {
      int v = E[i].v;
      E[i].u = ID[E[i].u];
      E[i].v = ID[E[i].v];
      if (E[i].u != E[i].v) E[i].cost -= dis[v];
    }
    NV = cntnode;
    root = ID[root];
  }
  return ret;
}

void Read() {
  while (scanf("%d%d", &n, &m) != EOF) {
    getchar();
    for (int i = 0; i < n; i++)
      scanf("%lf%lf", &p[i].x, &p[i].y);
      getchar();
      int ne = 0;
      for (int i = 0; i < m; i++) {
        scanf("%d%d", &E[ne].u, &E[ne].v);
        getchar();
        if (E[ne].u == E[ne].v) continue;
        E[ne].u--;
        E[ne].v--;
        E[ne].cost = distance(p[E[ne].u], p[E[ne].v]);
        ne++;
      }
      type ans = Directed_MST(0, n, ne);
      if (ans == -1) printf("poor snoopy\n");
      else printf("%.2f\n", ans);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
} 
