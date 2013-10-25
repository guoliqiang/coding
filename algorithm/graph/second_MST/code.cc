// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-22 08:14:09
// File  : code.cc
// Brief : no directed graph

#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int dis[MAX] = {0};
int visited[MAX] = {0};
int dp[MAX][MAX] = {{0}};
int pre[MAX] = {0};
int selected[MAX][MAX] = {{0}};


bool SMST(int & s_mst) {
  int mst = 0;
  s_mst = 0;
  memset(visited, 0, sizeof(visited));
  memset(dp, 0, sizeof(dp));
  memset(pre, 0, sizeof(pre));
  memset(selected, 0, sizeof(selected));

  for (int i = 0; i < N; i++) {
    if (matrix[0][i] > 0) {
      dis[i] = matrix[0][i];
      pre[i] = 0;
    } else dis[i] = -1;
  }
  dis[0] = 0;
  visited[0] = 1;
  pre[0] = -1;

  for (int i = 1; i < N; i++) {
    int min = INT_MAX;
    int idx = -1;
    for (int j = 0; j < N; j++) {
      if (visited[j] == 0 && dis[j] != -1 && dis[j] < min) {
        min = dis[j];
        idx = j;
      }
    }
    if (idx == -1) return false;
    
    selected[pre[idx]][idx] = 1;
    for (int j = 0; j < N; j++) {
      if (visited[j] == 1 && dp[j][idx] < matrix[pre[idx]][idx]) {
        dp[j][idx] = matrix[pre[idx]][idx];
      }
    }
    
    mst += min;
    visited[idx] = 1;
    for (int j = 0; j < N; j++) {
      if (visited[j] == 0 && ( dis[j] == -1 || matrix[idx][j] < dis[j])) {
        dis[j] = matrix[idx][j];
        pre[j] = idx;
      }
    }
  }
  LOG(INFO) << "MST:" << mst;
  s_mst = INT_MAX;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0 && selected[i][j] == 0) {
        s_mst = std::min(s_mst, mst + matrix[i][j] - dp[i][j]);
      }
    }
  }
  return true;
}


void Read(std::vector<std::vector<int> > & v) {
  N = v.size();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 3;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][1] = v[1][0] = 2;
  v[0][2] = v[2][0] = 1;
  v[1][2] = v[2][1] = 3;
  int s_mst = 0;
  Read(v);
  if (SMST(s_mst)) LOG(INFO) << s_mst;
  return 0;
}
