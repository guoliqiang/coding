// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-14 02:15:35
// File  : code.cc
// Brief :

/*  Accepted  240K  47MS
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 55 * 2;
int N = 0;
int matrix[MAXN][MAXN];
int flow[MAXN][MAXN];
int pre[MAXN];
int visited[MAXN];

int MaxFlow(int s, int t) {
  memset(flow, 0, sizeof(flow));
  memset(pre, -1, sizeof(pre));
  int n = N * 2;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) flow[i][j] = matrix[i][j];
  }
  int rs = 0;
  while (true) {
    memset(visited, 0, sizeof(visited));
    std::queue<int> queue;
    queue.push(s);
    visited[s] = 1;
    while (!queue.empty()) {
      int cur = queue.front();
      queue.pop();
      if (cur == t) break;
      for (int i = 0; i < n; i++) {
        if (flow[cur][i] > 0 && visited[i] == 0) {
          visited[i] = 1;
          pre[i] = cur;
          queue.push(i);
        }
      }
    }
    if (visited[t] == 0) break;
    int min = INF;
    for (int i = t; i != s; i = pre[i]) {
      min = std::min(min, flow[pre[i]][i]);
    }
    for (int i = t; i != s; i = pre[i]) {
      flow[pre[i]][i] -= min;
      flow[i][pre[i]] += min;
    }
    rs += min;
  }
  return rs;
}


void Read() {
  int n, m;
  while (scanf("%d %d*c", &n, &m) != EOF) {
    if (n == 1 && m == 0) {
      printf("1\n");
      continue;
    }
    if (m == 0) {
      printf("0\n");
      continue;
    }
    N = n;
    memset(matrix, 0, sizeof(matrix));
    int source = 0;
    for (int i = 0; i < m; i++) {
      while (getchar() != '(');
      int u, v;
      scanf("%d,%d", &u, &v);
      matrix[u][n + u] = matrix[v][n + v] = 1;
      matrix[N + u][v] = matrix[N + v][u] = INF;
      source = N + u;
    }
    getchar();
    int flow = INF;
    int target = 0;
    for (int i = 0; i < N; i++) {
      if (i + N == source) continue;
      int t = MaxFlow(source, i);
      if (t < flow) {
        flow = t;
        target = i;
      }
    }
    if (flow == INF) printf("%d\n", n);
    else printf("%d\n", flow);
  }
}

}  // namespace algorithm


using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
