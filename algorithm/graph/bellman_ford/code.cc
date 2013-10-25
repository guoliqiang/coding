// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-24 23:02:18
// File  : code.cc
// Brief :
// 在有边值为负数的情况下求单源最短路径
// Note: 单源最短路径的定义并没有规定路径中只能经过某个点一次
#include "base/public/common_head.h"

namespace algorithm {
#define INF 0x3f3f3f3f

const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int dis[MAX] = {0};
int pre[MAX];

int BellmanFord(int source) {
  memset(dis, 0, sizeof(dis));
  memset(pre, 0xff, sizeof(pre));
  for (int i = 0; i < N; i++) {
    if (matrix[source][i] > 0) {
      dis[i] = matrix[source][i];
      pre[i] = source;
    }
    else dis[i] = INF;
  }
  dis[source] = 0;
  for (int k = 1; k < N; k++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (matrix[i][j] != 0 && dis[j] > dis[i] + matrix[i][j]) {
          dis[j] = dis[i] + matrix[i][j];
          pre[j] = i;
        }
      }
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] != 0 && dis[j] > dis[i] + matrix[i][j]) return 0;  // 存在负权回路
    }
  }
  return 1;
}

void Read(std::vector<std::vector<int> > & v) {
  memset(matrix, 0, sizeof(matrix));
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
  int n = 7;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][1] = 6;
  v[0][2] = 5;
  v[0][3] = 5;
  v[3][2] = -2;
  v[2][1] = -2;
  v[1][4] = -1;
  v[2][4] = -1;
  v[3][5] = -1;
  v[4][6] = 3;
  v[5][6] = 3;
  Read(v);
  BellmanFord(0);
  for(int i = 0; i < N; i++) LOG(INFO) << "dis[" << i << "]:" << dis[i];
  return 0;
}
