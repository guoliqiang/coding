// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-14 21:27:49
// File  : code.cc
// Brief :

/*
 * Accepted 406 ms  1615 kb
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 210;
int N = 0;
int low[MAXN][MAXN];
int up[MAXN][MAXN];
int du[MAXN];

int visited[MAXN];
int pre[MAXN];
int flow[MAXN][MAXN];

bool MaxFlow() {
  int rs = 0;
  int s = 201;
  int t = 202;
  memset(flow, 0, sizeof(flow));
  int full = 0;
  for (int i = 0; i < N; i++) {
    if (du[i] > 0) {
      flow[s][i] = du[i];
      full += du[i];
    } else if (du[i] < 0) flow[i][t] = -du[i];
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      flow[i][j] = up[i][j] - low[i][j];
    }
  }
  while (true) {
    memset(visited, 0, sizeof(visited));
    memset(pre, 0, sizeof(pre));
    std::queue<int> queue;
    queue.push(s);
    visited[s] = 1;
    while (!queue.empty()) {
      int cur = queue.front();
      queue.pop();
      if (cur == t) break;
      for (int i = 0; i < MAXN; i++) {
        if (flow[cur][i] > 0 && visited[i] == 0) {
          visited[i] = 1;
          queue.push(i);
          pre[i] = cur;
        }
      }
    }
    if (visited[t] == 0) break;
    int min = INF;
    for (int i = t; i != s; i = pre[i]) {
      min = std::min(flow[pre[i]][i], min);
    }
    for (int i = t; i != s; i = pre[i]) {
      flow[pre[i]][i] -= min;
      flow[i][pre[i]] += min;
    }
    rs += min;
  }
  return rs == full;
}

void Read() {
  int n, m;
  while (scanf("%d%d", &n, &m) != EOF) {
    std::queue<std::pair<int, int> > edges;
    getchar();
    N = n;
    memset(low, 0, sizeof(low));
    memset(up, 0, sizeof(up));
    memset(du, 0, sizeof(du));
    for (int i = 0; i < m; i++) {
      int u, v, s, b;
      scanf("%d%d%d%d", &u, &v, &s, &b);
      getchar();
      low[u - 1][v - 1] = s;
      up[u - 1][v - 1] = b;
      du[u - 1] -= s;
      du[v - 1] += s;
      edges.push(std::make_pair(u - 1, v - 1));
    }
    if (MaxFlow()) {
      printf("YES\n");
      while (!edges.empty()) {
        std::pair<int, int> cur = edges.front();
        edges.pop();
        int u = cur.first;
        int v = cur.second;
        if (flow[u][v] > up[u][v] - low[u][v]) {
          printf("%d\n", low[u][v]);
        } else {
          printf("%d\n", low[u][v] + (up[u][v] - low[u][v] - flow[u][v]));
        }
      }
    } else {
      printf("NO\n");
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
