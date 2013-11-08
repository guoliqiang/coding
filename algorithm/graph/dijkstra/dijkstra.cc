// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-10 10:33:55
// File  : dijkstra.cc
// Brief :

#include "base/public/common_head.h"
#include "../base/graph.h"

namespace NB {
const int INF = 0x3f3f3f3f;

void dijkstra(std::vector<std::vector<int> > matrix, int source,
              std::vector<int> & dist, std::vector<int> & path) {
  int n = matrix.size();
  dist.resize(n, INF);
  path.resize(n, -1);
  std::vector<bool> visited(n, false);
  
  dist[source] = 0;  // 初始化只有这一个就可以了
  
  for (int j = 0; j < n; j++) {  // note begin from 0
    int min = INF;
    int idx = -1;
    for (int i = 0; i < n; i++) {
      if (visited[i] == false && dist[i] < min) {
        min = dist[i];
        idx = i;
      }
    }
    if (idx == -1) break;
    visited[idx] = true;
    for (int i = 0; i < n; i++) {
      if (matrix[idx][i] > 0 && dist[i] > dist[idx] + matrix[idx][i]) {
        dist[i] = dist[idx] + matrix[idx][i];
        path[i] = idx;
      }
    }
  }
}

}  // namespace NB


namespace graph {

void Dijkstra(std::vector<std::vector<int> > matrix, int source,
              std::vector<int> & dist, std::vector<int> & path) {
  int n = matrix.size();
  dist.resize(n, -1);
  path.resize(n, -1);
  std::vector<bool> visited(n, false);
  // 初始化
  for (int i = 0; i < n; i++) {
    if (matrix[source][i] > 0 && (dist[i] == -1 || dist[i] < matrix[source][i])) {  // bug fixed, 如果没有dist[i] < matrix[source][i]，poj上2449无法AC
      dist[i] = matrix[source][i];
      path[i] = source;
    }
    else dist[i] = -1;
  }
  dist[source] = 0;
  visited[source] = true;
  
  // 一次加入一个点，由于source已经计算完毕，还剩下
  // n - 1个点
  for (int j = 1; j < n; j++) {
    // 从未visited的点中选出一个距离最近的
    int min = ~(1 << 31);
    int idx = -1;
    for (int i = 0; i < n; i++) {
      if (visited[i] == false && dist[i] != -1 && dist[i] < min) {
        min = dist[i];
        idx = i;
      }
    }
    if (idx == -1) break;
    // 基于选出来的点，更新dist数组
    for (int i = 0; i < n; i++) {
      if (visited[i] == false && 
          matrix[idx][i] > 0  && 
          (dist[i] == -1 || dist[i] > dist[idx] + matrix[idx][i])) {
        dist[i] = dist[idx] + matrix[idx][i];
        path[i] = idx;
      }
    }
    visited[idx] = true;
  }
}

void DijkstraSimple(std::vector<std::vector<int> > & matrix, int source,
                    std::vector<int> & dist, std::vector<int> & path) {
  static const int INI_MAX = ~(1 << 31);
  int n = matrix.size();
  dist.resize(n, INI_MAX);
  path.resize(n, -1);
  std::vector<bool> visited(n, false);
  for (int i = 0; i < n; i++) {
    if (matrix[source][i] > 0) {
      dist[i] = matrix[source][i];
      path[i] = source;
    }
  }
  dist[source] = 0;
  visited[source] = true;

  for (int j = 1; j < n; j++) {
    int min = INT_MAX;
    int u = -1;
    for (int i = 0; i < n; i++) {
      if (visited[i] == false && dist[i] < min) {
        min = dist[i];
        u = i;
      }
    }
    if (u == -1) break;
    for (int i = 0; i < n; i++) {
      if (visited[i] == false &&
          matrix[u][i] > 0 &&
          dist[i] > dist[u] + matrix[u][i]) {
        dist[i] = dist[u] + matrix[u][i];
      }
    }
    visited[u] = true;
  }
}

}  // namespace graph

using namespace graph;
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix;
  AdjaMatrix(matrix, 5);
  SetMatrix(matrix, 0, 1, 2);
  SetMatrix(matrix, 0, 2, 5);
  SetMatrix(matrix, 0, 3, 1);
  SetMatrix(matrix, 1, 4, 3);
  SetMatrix(matrix, 2, 4, 4);
  SetMatrix(matrix, 3, 4, 2);
  SetMatrix(matrix, 1, 2, 4);
  SetMatrix(matrix, 2, 3, 3);
  std::vector<int> dist;
  std::vector<int> path;
  Dijkstra(matrix, 0, dist, path);
  LOG(INFO) << JoinVector(dist);
  LOG(INFO) << JoinVector(path);
  DijkstraSimple(matrix, 0, dist, path);
  LOG(INFO) << JoinVector(dist);
  LOG(INFO) << JoinVector(path);
  return 0;
}
