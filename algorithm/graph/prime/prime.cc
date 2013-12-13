// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-10 11:32:31
// File  : prime.cc
// Brief :
/*
 * prime算法的基本思想
 * 1.清空生成树，任取一个顶点加入生成树
 * 2.在那些一个端点在生成树里，另一个端点不在生成树里的边中，选取一条权最小的边，将它和另一个端点加进生成树
 * 3.重复步骤2，直到所有的顶点都进入了生成树为止.
 *
 * poj2485
 * */

#include "base/public/common_ojhead.h"
#include "../base/graph.h"

namespace algorithm {

// 这个navie法复杂度高
int PrimeNavie(std::vector<std::vector<int> > & matrix, std::vector<std::pair<int, int> > & rs) {
  int n = matrix.size();
  std::vector<bool> visited(n, false);
  visited[0] = true;
  int sum = 0;
  for (int j = 1; j < n; j++) {
    int min = ~(1 << 31);
    int u1 = -1;
    int u2 = -1;
    for (int i = 0; i < n; i++) {
      if (visited[i] == false) continue;
      for (int k = 0; k < n; k ++) {
        if (visited[k] == true) continue;
        if (matrix[i][k] > 0 && matrix[i][k] < min) {
          min = matrix[i][k];
          u1 = i;
          u2 = k;
        }
      }
    }
    sum += matrix[u1][u2];
    if (u1 == -1) break;
    visited[u2] = true;
    rs.push_back(std::make_pair(u1, u2));
  }
  return sum;
}

int Prime(std::vector<std::vector<int> > & matrix, int cur, std::vector<std::pair<int, int> > & rs) {
  int n = matrix.size();
  std::vector<int> dist(n, INT_MAX);
  std::vector<bool> visited(n, false);
  std::vector<int> pre(n, -1);
  int sum = 0;
  // 初始化
  for (int i = 0; i < n; i++) {
    if (matrix[cur][i] > 0) {
      dist[i] = matrix[cur][i];
      pre[i] = cur;
    }
  }
  visited[cur] = true;
  
  // 共n - 1条边，n个点
  for (int j = 1; j < n; j++) {
    int min = INT_MAX;
    int u = -1;
    for (int i = 0; i < n; i ++) {
      if (visited[i] == false && dist[i] < min) {
        min = dist[i];
        u = i;
      }
    }
    if (u == -1) break;
    visited[u] = true;
    sum += dist[u];
    rs.push_back(std::make_pair(pre[u], u));
    // update
    for (int i = 0; i < n; i++) {
      if (visited[i] == false && matrix[u][i] > 0 && dist[i] > matrix[u][i]) {  // Note this line
        dist[i] = matrix[u][i];
        pre[i] = u;
      }
    }
  }
  return sum;
}
}  // namespace algorithm

namespace algorithm {
const int MAXN = 1000;
int N;
int matrix[MAXN][MAXN];
int dis[MAXN];
int visited[MAXN];
int pre[MAXN];

int Prime(int source = 0) {
  int rs = 0;
  memset(visited, 0, sizeof(visited));
  memset(pre, 0, sizeof(pre));
  for (int i = 0; i < N; i++) dis[i] = INF;
  dis[source] = 0;
  for (int k = 0; k < N; k++) {  // note begin from 0
    int min = INF;
    int index = -1;
    for (int i = 0; i < N; i++) {
      if (visited[i] == 0 && dis[i] < min) {
        min = dis[i];
        index = i;
      }
    }
    if (index == -1) break;
    visited[index] = 1;
    rs += dis[index];
    for (int i = 0; i < N; i++) {
      if (visited[i] == 0 && matrix[index][i] > 0 && dis[i] > matrix[index][i]) {
        dis[i] = matrix[index][i];
        pre[i] = index;
      }
    }
  }
  return rs;
}
}  // namespace algorithm

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
  std::vector<std::pair<int, int> > rs;
  LOG(INFO) << Prime(matrix, 0, rs);
  for (int i = 0; i < rs.size(); i++) LOG(INFO) << rs[i].first << " " << rs[i].second;
  rs.clear();
  LOG(INFO) << PrimeNavie(matrix, rs);
  for (int i = 0; i < rs.size(); i++) LOG(INFO) << rs[i].first << " " << rs[i].second;
  return 0;
}

