// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-14 20:59:40
// File  : code.cc
// Brief :

// http://blog.csdn.net/smartxxyx/article/details/9293665
// 图的最大流等于其最小割

// http://blog.csdn.net/kksleric/article/details/7429849
// 怎样求割：求完最大流后，在残留网络中从source开始dfs，被染色的为S，未被染色的为T，则边集[S,T]为割。

#include "base/public/common_head.h"

// 增广路径是没有环的

namespace algorithm {

static const int MAX = 1000;
int matrix[MAX][MAX];
int flow[MAX][MAX];  // flow[i][j] 表示残留网络中i --> j 还有多少流量可供流
int pre[MAX];
int visited[MAX];
int n;

int MaxFlow(int source, int target) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; i++)
      flow[i][j] = matrix[i][j];  // 如果是无向图flow[j][i] = matrix[i][j]
  }
  int rs = 0;
  // 循环寻找增广路径
  while (true) {
    memset(pre, 0, sizeof(pre));
    memset(visited, 0, sizeof(visited));

    std::queue<int> queue;
    visited[source] = 1;
    queue.push(source);
    while (!queue.empty()) {
      int t = queue.front();
      queue.pop();
      if (t == target) break;
      for (int i = 0; i < n; i++) {
        if (!visited[i] && flow[t][i] > 0) {
          visited[i] = 1;
          queue.push(i);
          pre[i] = t;
        }
      }
    }
    if (visited[target] != true) break;
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

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
