// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-14 21:30:43
// File  : code.cc
// Brief : poj1966 
// 最少去掉多少条边，能是图变得不连通 (可以看出：这样的图，如果存在桥边，结果就为1)
// 边连通度  ==  最小割  == 最大流

// 若G为无向图：
// 1.     原G图中的每条边e=UV变成两条边e`=UV,e``=VU,容量都为1；
// 2.     固定一个点为源点，枚举与汇点，求最大流max_flow，保留最小的max_flow即为图的边连通度。
// 若G为有向图：
// 1.     原G图中每条有向边容量为1
// 2.     固定一个点为源点，枚举与汇点，求最大流max_flow，保留最小的max_flow即为图的边连通度。
// 这里说的有向图的连通性是强连通
// 注意固定源点(任意选)，遍历汇点

#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
int n;
int matrix[MAX][MAX];
int visited[MAX];
int pre[MAX];
int flow[MAX][MAX];
int cut[MAX][MAX];

int MaxFlow(int source, int target) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (matrix[i][j] > 0) flow[i][j] = 1; // 如果是无向图需要设置flow[j][i] = 1
    }
  }

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
  int rs = INT_MAX;
  int target = - 1;
  for (int i = 1; i < n; i++) {
    int tmp = MaxFlow(0, i);
    if (tmp < rs) {
      rs = tmp;
      target = i;
    }
  }
  int tmp = rs;
  // 循环删除边看是否回影响最大流
  // 如果有影响，说明此点是割边
  memset(cut, 0, sizeof(cut));
  for (int i = 0; i < n && tmp; i++) {
    for (int j = 0; j < n && tmp; j++) {
      if (matrix[i][j] > 0) {
        matrix[i][j] = 0;
        int cur = MaxFlow(0, target);
        if (cur < rs) {
          cut[i][j] = 1;
          tmp--;
        }
        else matrix[i][j] = 1;
      }
    }
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
