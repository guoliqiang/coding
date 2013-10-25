// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-12 21:04:28
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

int Find(std::vector<int> & v, int i) {
  return v[i] = (v[i] == i) ? i : Find(v, v[i]);
}

// edge x - > y
void Merge(std::vector<int> & v, int x, int y) {
  int p_x = Find(v, x);
  int p_y = Find(v, y);
  if (p_x != p_y) v[p_x] = p_y;
}

// 只适用于有向图
bool ConnectedUFind(std::vector<std::vector<int> > & matrix) {
  int n = matrix.size();
  std::vector<int> v(n, 0);
  for (int i = 0; i < n; i++) v[i] = i;
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (matrix[i][j] > 0) Merge(v, i, j);
    }
  }
  int root = v[0];
  for (int i = 1; i < n; i++) {
    if (root != Find(v, i)) return false;
  }
  return true;
}

void ConnectedDFS(std::vector<std::vector<int> > & matrix,
                  std::vector<bool> & visited, int k, int & cnt) {
  visited[k] = true;
  cnt++;
  for (int i = 0; i < matrix.size(); i++) {
    if (matrix[k][i] > 0 && visited[i] == false)
      ConnectedDFS(matrix, visited, i, cnt);
  }
}

// 有／无向图都可以
bool ConnectedDFS(std::vector<std::vector<int> > & matrix) {
  int cnt = 0;
  std::vector<bool> visited(matrix.size(), false);
  ConnectedDFS(matrix, visited, 0, cnt);
  return cnt == matrix.size();
}

// 有／无向图都可以
bool ConnectedBFS(std::vector<std::vector<int> > & matrix) {
  int cnt = 0;
  std::vector<bool> visited(matrix.size(), false);
  std::queue<int> queue;
  queue.push(0);
  while (!queue.empty()) {
    int t = queue.front();
    queue.pop();
    cnt++;
    visited[t] = true;
    for (int i = 0; i < matrix.size(); i++) {
      if (matrix[t][i] > 0 && visited[i] == false) {
        queue.push(i);
      }
    }
  }
  return cnt == matrix.size();
}

/*
 * 不能用拓扑排序
struct TNode {
  int val;
  int vertex;
};

bool TCmp(const TNode * p1, const TNode * p2) {
  return p1->val < p2-<val;
}
bool ConnectedTopology(std::vector<std::vector<int> > & matrix) {
  std::vector<TNode> income;
  std::priority_queue<TNode*, std::vector<TNode*>, Cmp> heap;
  int n = matrix.size();
  for (int i = 0; i < n; i++) {
    int t = 0;
    for (int j = 0; j < n; j++) {
      if (matrix[j][])
    }
  }
}
*/
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
