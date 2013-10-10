// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-10 11:32:31
// File  : prime.cc
// Brief :

#include "base/public/common_head.h"
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

