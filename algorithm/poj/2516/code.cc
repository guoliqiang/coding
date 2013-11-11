// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-11 02:46:53
// File  : code.cc
// Brief :

/*
 * Accepted 4244K 579MS
 *
 * */

#include "base/public/common_ojhead.h"


namespace algorithm {
const int MAXK = 50;
const int MAXN = 102;
int N = 0;
int flow[MAXK][MAXN][MAXN];
int cost[MAXK][MAXN][MAXN];
int pre[MAXN];
int store[MAXK][MAXK];
int need[MAXK][MAXK];
int sum[MAXK];
int weight[MAXN];
int visited[MAXN];  // 用于加速

int MaxMinFlow(int k, int s, int t, int *mcost) {
  int rs = 0;
  while (true) {
    for (int i = 0; i < MAXN; i++) weight[i] = INF;
    memset(pre, -1, sizeof(pre));
    memset(visited, 0, sizeof(visited));
    std::queue<int> queue;
    queue.push(s);
    weight[s] = 0;
    visited[s] = 1;
    while (!queue.empty()) {
      int cur = queue.front();
      queue.pop();
      visited[cur] = 0;
      for (int i = 0; i < MAXN; i++) {
        if (flow[k][cur][i] > 0 && weight[i] > weight[cur] + cost[k][cur][i]) {
          weight[i] = weight[cur] + cost[k][cur][i];
          pre[i] = cur;
          if (visited[i] == 0) {
            queue.push(i);
            visited[i] = 1;
          }
        }
      }
    }
    if (pre[t] == -1) break;
    int min = INF;
    for (int i = t; i != s; i = pre[i]) {
      min = std::min(min, flow[k][pre[i]][i]);
    }
    for (int i = t; i != s; i = pre[i]) {
      flow[k][pre[i]][i] -= min;
      flow[k][i][pre[i]] += min;
    }
    *mcost += min * weight[t];
    rs += min;
  }
  return rs;
}

void Read() {
  int n = 0;
  int m = 0;
  int k = 0;
  while (scanf("%d%d%d", &n, &m, &k) != EOF) {
    getchar();
    if (n == 0 && m == 0 && k == 0) break;
    memset(flow, 0, sizeof(flow));
    memset(cost, 0, sizeof(cost));
    memset(store, 0, sizeof(store));
    memset(need, 0, sizeof(need));
    memset(sum, 0, sizeof(sum));
    // n个need
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < k; j++) {
        int t = 0;
        scanf("%d", &t);
        need[i][j] = t;
        sum[j] += t;
      }
      getchar();
    }

    // m个supply 
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < k; j++) {
        int t = 0;
        scanf("%d", &t);
        store[i][j] = t;
      }
      getchar();
    }
    //
    for (int i = 0; i < k; i++) {  // goods
      for (int j = 0; j < n; j++) { // need
        for (int l = 0; l < m; l++) { // supply
          flow[i][l][50 + j] = INF;
          int t = 0;
          scanf("%d", &t);
          cost[i][l][50 + j] = t;
          cost[i][50 + j][l] = -t;  // 注意初始化负费用，用于增广路径的回流，之前一直没有意识到此问题，一直WA
        }
        getchar();
      }
    }
    //
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < m; j++) {  // supply
        flow[i][100][j] = store[j][i];
      }
    }
    for (int i = 0; i < k; i++) {  // goods
      for (int j = 0; j < n; j++) {  // need
        flow[i][50 + j][101] = need[j][i];
      }
    }
    //
    bool flag = true;
    int mcost = 0;
    for (int i = 0; i < k; i++) {
      int t = MaxMinFlow(i, 100, 101, &mcost);
      if (t != sum[i]) {
        flag = false;
        break;
      }
    }
    if (flag) printf("%d\n", mcost);
    else printf("-1\n");
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
