// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-14 21:30:43
// File  : code.cc
// Brief :
// 最少去掉多少条边，能是图变得不连通
// 边连通度  ==  最小割  == 最大流
#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
int n;
int matrix[MAX][MAX];
int visited[MAX];
int pre[MAX];
int flow[MAX][MAX];

int MaxFlow(int source, int target) {
  int rs = 0;
  while (true) {
    memset(pre, 0, sizeof(pre));
    memset(visited, 0, sizeof(visited));
    std::queue<int> queue;
    queue.push(source);
    visited[source] = 1;
    while (!queue.empty()) {
      int t = queue.front();
      queue.pop();
      if (t == target) break;
      for (int i = 0; i < n; i++) {
        if (visited[i] == 0 && flow[t][i] > 0) {
          pre[i] = t;
          queue.push(i);
        }
      }
    }
    if (visited[target] == 0) break;
    int min = INT_MAX;
    for (int i = target; i != source; i = pre[i]) {
      min = std::min(min, flow[pre[i]][i]);
    }
    for (int i = target; i != source; i = pre[i]) {
      flow[pre[i]][i] -= min;
      flow[i][pre[i]] += min;
    }
    rs += min;
  }
  return rs;
}

int EdgeConnectedDegree() {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (matrix[i][j] > 0) flow[i][j] = 1;  // 如果是无向图需要设置flow[j][i] = 1
    }
  }
  int rs = INT_MAX;
  for (int i = 1; i < n; i++) {
    rs = std::min(rs, MaxFlow(0, i));
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
