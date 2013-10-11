// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-11 09:31:32
// File  : code.cc
// Brief :
/*
 * A* 算法，可能有环
 * 如果可以没有环，感觉可以使用floyd算法
 * http://blog.sciencenet.cn/blog-5422-538894.html
 * http://blog.csdn.net/zhjchengfeng5/article/details/7858194
 *
 * T , I do not know why?
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

void Dijkstra(std::vector<std::vector<int> > & matrix, int source, std::vector<int> & dist) {
  int n = matrix.size();
  dist.resize(n, INT_MAX);
  std::vector<bool> visited(n, false);
  for (int i = 0; i < n; i++) {
    if (matrix[source][i] > 0) dist[i] = matrix[source][i];
  }
  dist[source] = 0;
  visited[source] = true;
  for (int i = 1; i < n; i++) {
    int u = -1;
    int min = INT_MAX;
    for (int i = 0; i < n; i++) {
      if (visited[i] == false && dist[i] > min) {
        min = dist[i];
        u = i;
      }
    }
    if (u == -1) break;
    visited[u] = true;
    // reatch here, dist[u] can not be INT_MAX
    for (int i = 0; i < n; i++) {
      if (visited[i] == false && matrix[u][i] > 0 && dist[i] > dist[u] + matrix[u][i])
        dist[i] = dist[u] + matrix[u][i];
    }
  }
}

struct Node {
  int val;
  int len;
  int h;

  Node() {}
  Node(int v, int l, int h_i) : val(v), len(l), h(h_i){}

  bool operator < (const Node & a) const {
    if (len + h == a.len + a.h) return len < a.len;  // Note
    return len + h < a.len + a.h;
  }
};

int AStar(std::vector<std::vector<int> > & matrix, int start, int end, int k) {
  if (start == end) k++;  // for poj specified
  int n = matrix.size();
  std::vector<std::vector<int> > tmatrix(n, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) tmatrix[i][j] = matrix[j][i];
  }
  std::vector<int> dist;
  Dijkstra(tmatrix, end, dist);
  if (dist[start] == INT_MAX) return -1;
  std::priority_queue<Node> queue;
  queue.push(Node (start, 0, dist[start]));
  int cnt = 0;
  while (!queue.empty()) {
    Node t = queue.top();
    queue.pop();
    if (t.val == end) cnt++;
    if (cnt == k) return t.len;
    for (int i = 0; i < n; i++) {
      if (matrix[t.val][i] > 0 && dist[i] != INT_MAX) {
        queue.push(Node(i, t.len + matrix[t.val][i], dist[i]));
      }
    }
  }
  return -1;
}

int AStar(std::vector<std::vector<int> > & matrix, std::vector<std::vector<int> > & tmatrix,
          int start, int end, int k) {
  if (start == end) k++;  // for poj specified
  
  int n = matrix.size();
  std::vector<int> dist;
  Dijkstra(tmatrix, end, dist);
  if (dist[start] == INT_MAX) return -1;
  
  std::priority_queue<Node> queue;
  queue.push(Node (start, 0, dist[start]));
  int cnt = 0;
  while (!queue.empty()) {
    Node t = queue.top();
    queue.pop();
    if (t.val == end) cnt++;
    if (cnt == k) return t.len;
    for (int i = 0; i < n; i++) {
      if (matrix[t.val][i] > 0 && dist[i] != INT_MAX) {
        queue.push(Node(i, t.len + matrix[t.val][i], dist[i]));
      }
    }
  }
  return -1;
}

void ReadFromConsole() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;
  std::vector<std::vector<int> > matrix(n, std::vector<int>(n, 0));
  std::vector<std::vector<int> > tmatrix(n, std::vector<int>(n, 0));
  for (int i = 0; i < m; i++) {
    int i1 = 0;
    int i2 = 0;
    int v = 0;
    std::cin >> i1 >> i2 >> v;
    matrix[i1 - 1][i2 - 1] = v;
    tmatrix[i2 - 1][i1 - 1] = v;
  }
  int start = 0;
  int end = 0;
  int k = 0;
  std::cin >> start >> end >> k;
  std::cout << AStar(matrix, tmatrix, start - 1, end - 1, k);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 2;
  std::vector<std::vector<int> > matrix(n, std::vector<int>(n, 0));
  matrix[0][1] = 5;
  matrix[1][0] = 4;
  LOG(INFO) << AStar(matrix, 0, 1, 2);
  ReadFromConsole();
  return 0;
}
