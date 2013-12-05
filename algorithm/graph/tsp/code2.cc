// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-03 17:30:11
// File  : code2.cc
// Brief :

// 分支限界发求tsp问题

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1000;
int N = 0;
int matrix[MAXN][MAXN];
struct Node {
  int path[MAXN];
  int cur;
  int val;
  int cost;
};

int out[MAXN];
int path[MAXN];
int rs;
// 最小堆
class Cmp {
 public:
  bool operator () (const Node & x, const Node & y) const {
    return x.val + x.cost > y.val + y.cost;
  }
};

bool Trace(int begin) {
  rs = INF;
  // out和sum用于求下届
  for (int i = 0; i < N; i++) out[i] = INF;
  for (int i = 0; i < N; i++) {
    int min = INF;
    for (int j = 0; j < N; j++) {
      if(matrix[i][j] > 0 && min > matrix[i][j]) min = matrix[i][j];
    }
    if (min == INF) return false;  // 无法形成回路
    out[i] = min;
  }
  int sum = 0;
  for (int i = 0; i < N; i++) sum += out[i];
  std::priority_queue<Node, std::vector<Node>, Cmp> queue;
  
  Node t;
  for (int i = 0; i < N; i++) t.path[i] = i;
  std::swap(t.path[0], t.path[begin]);
  t.cur = 0;
  t.val = 0;
  t.cost = sum;
  queue.push(t);
  
  while (!queue.empty()) {
    Node foo = queue.top();
    queue.pop();
    if (foo.cur == N - 1 && matrix[foo.path[N - 1]][foo.path[0]] > 0
        && foo.val + matrix[foo.path[N - 1]][foo.path[0]] < rs) {
      rs = foo.val + matrix[foo.path[N - 1]][foo.path[0]];
      for (int i = 0; i < N; i++) path[i] = foo.path[i];
    } else {
      for (int i = foo.cur + 1; i < N; i++) {
        int u = foo.path[foo.cur];
        int v = foo.path[i];
        if (matrix[u][v] == 0) continue;
        int curval = foo.val + matrix[u][v] + foo.cost - out[u];
        if (curval >= rs) continue;
        Node bar;
        bar.cur = foo.cur + 1;
        bar.cost = foo.cost - out[u];
        bar.val = foo.val + matrix[u][v];
        for (int j = 0; j < N; j++) bar.path[j] = foo.path[j];
        std::swap(bar.path[bar.cur], bar.path[i]);
        queue.push(bar);
      }
    }
  }
  return true;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  N = 3;
  matrix[0][1] = matrix[1][0] = 2;
  matrix[1][2] = 2;
  matrix[2][1] = 1;
  matrix[2][0] = matrix[0][2] = 1;
  LOG(INFO) << Trace(0);
  LOG(INFO) << rs;
  for (int i = 0; i < N; i++) LOG(INFO) << path[i];
  return 0;
}
