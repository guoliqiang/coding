// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-10 22:28:15
// File  : code.cc
// Brief :


#include "base/public/common_ojhead.h"

namespace NB {
/*
 * Accepted 688K  719MS C++
 * */
const int MAXN = 372;
int N = 0;
int flow[MAXN][MAXN];
int visited[MAXN];
int pre[MAXN];

int MaxFlow(int s, int t) {
  int rs = 0;
  while (true) {
    memset(visited, 0, sizeof(visited));
    memset(pre, -1, sizeof(pre));
    std::queue<int> queue;
    queue.push(s);
    visited[s] = 1;
    while (!queue.empty()) {
      int cur = queue.front();
      queue.pop();
      if (cur == t) break;
      for (int i = 0; i < N; i++) {
        if (flow[cur][i] > 0 && visited[i] == 0) {
          queue.push(i);
          visited[i] = 1;
          pre[i] = cur;
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
  int k = 0;
  int temp[9];
  scanf("%d", &k);
  getchar();
  for (int i = 0; i < k; i++) {
    memset(flow, 0, sizeof(flow));
    int n = 0;
    int maxw = 0;
    int f = 0;
    scanf("%d", &n);
    getchar();
    for (int j = 0; j < n; j++) {
      for (int l = 0; l < 9; l++) {
        scanf("%d", &(temp[l]));
      }
      getchar();
      f += temp[7];
      flow[0][j + 2] = temp[7]; // source --> film
      for (int p = 0; p < temp[8]; p++) {
        int offset = 2 + n;
        for (int l = 0; l < 7; l++) {
          if (temp[l] == 0) continue;
          int idx = p * 7 + offset + l;
          flow[j + 2][idx] = 1; // film -- > day
          flow[idx][1] = 1; // day --> target
        }
      }
      maxw = std::max(maxw, temp[8]);
    }
    N = 2 + n + 7 * maxw;
    int flow = MaxFlow(0, 1);
    if (flow == f) printf("Yes\n");
    else printf("No\n");
  }
}

}  // namespace NB

namespace algorithm {
// TLE
const int MAXM = 15000;
const int MAXN = 500;
int N = 0;
struct Node {
  int u;
  int v;
  int c;
  int next;
} edge[MAXM];
int head[MAXN];
int visited[MAXN];
int pre[MAXN];
int num = 0;

int MaxFlow(int source, int target) {
  int rs = 0;
  while (true) {
    memset(visited, 0, sizeof(visited));
    memset(pre, 0, sizeof(pre));
    std::queue<int> queue;
    queue.push(source);
    visited[source] = 1;
    while (!queue.empty()) {
      int t = queue.front();
      if (t == target) break;
      queue.pop();
      for (int i = head[t]; i != -1; i = edge[i].next) {
        if (edge[i].c > 0 && visited[edge[i].v] == 0) {
          visited[edge[i].v] = 1;
          queue.push(edge[i].v);
          pre[edge[i].v] = t;
        }
      }
    }
    if (visited[target] == 0) break;
    int min = INF;
    
    for (int i = target; i != source; i = pre[i]) {
      int t = pre[i];
      for (int j = head[t]; j != -1; j = edge[j].next) {
        if (edge[j].v == i) {
          min = std::min(min, edge[j].c);
          continue;
        }
      }
    }
    /*
    std::cout << min << " :";
    for (int i = target; i != source; i = pre[i]) {
      std::cout << i << "<--";
    }
    std::cout << source << std::endl;
    */
    for (int i = target; i != source; i = pre[i]) {
      int t = pre[i];
      for (int j = head[t]; j != -1; j = edge[j].next) {
        if (edge[j].v == i) {
          edge[j].c -= min;
          continue;
        }
      }
      for (int j = head[i]; j != -1; j = edge[j].next) {
        if (edge[j].v == t) {
          edge[j].c += min;
          continue;
        }
      }
    }
    rs += min;
  }
  return rs;
}

void Add(int u, int v, int c) {
  int idx = -1;
  for (int i = head[u]; i != -1; i = edge[i].next) {
    if (edge[i].v == v) idx = i;
  }
  if (idx != -1 && v != 1) {
    // LOG(INFO) << idx << " " << u << " " << v << " " << edge[idx].u << " " << edge[idx].v;
    edge[idx].c += c;
  } else {
    edge[num].u = u;
    edge[num].v = v;
    edge[num].c = c;
    edge[num].next = head[u];
    head[u] = num++;
  }

  idx = -1;
  for (int i = head[v]; i != -1; i = edge[i].next) {
    if (edge[i].v == u) idx = i;
  }
  if (idx == -1) {
    edge[num].u = v;
    edge[num].v = u;
    edge[num].c = 0;
    edge[num].next = head[v];
    head[v] = num++;
  }
}

void Read() {
  int k = 0;
  int temp[9];
  scanf("%d", &k);
  getchar();
  for (int i = 0; i < k; i++) {
    memset(edge, 0, sizeof(edge));
    memset(head, -1, sizeof(head));
    num = 0;
    int n = 0;
    int maxw = 0;
    int f = 0;
    scanf("%d", &n);
    getchar();
    for (int j = 0; j < n; j++) {
      for (int l = 0; l < 9; l++) {
        scanf("%d", &(temp[l]));
      }
      getchar();
      f += temp[7];
      Add(0, j + 2, temp[7]);  // source --> film
      for (int p = 0; p < temp[8]; p++) {
        int offset = 2 + n;
        for (int l = 0; l < 7; l++) {
          if (temp[l] == 0) continue;
          int idx = p * 7 + offset + l;
          Add(j + 2, idx, 1);  // film -- > day
          Add(idx, 1, 1);      // day --> target
        }
      }
      maxw = std::max(maxw, temp[8]);
    }
    N = 2 + n + 7 * maxw;
    int flow = MaxFlow(0, 1);
    if (flow == f) printf("Yes\n");
    else printf("No\n");
    // LOG(INFO) << "max flow:" << flow;
    // LOG(INFO) << "maxw :" << maxw << " N:" << N;
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  NB::Read();
  return 0;
}


