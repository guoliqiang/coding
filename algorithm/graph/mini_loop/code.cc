// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-24 02:43:03
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
#define INF 0x3f3f3f3f
const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int dis[MAX][MAX] = {{0}};
int pre[MAX][MAX] = {{0}};
int out[MAX] = {0};
int cnt = 0;

void MiniLoop() {
  cnt = 0;
  memset(dis, 0, sizeof(dis));
  memset(pre, 0, sizeof(pre));
  memset(out, 0, sizeof(out));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      pre[i][j] = i;
      if (i == j) dis[i][j] = 0;
      else if (matrix[i][j] > 0) dis[i][j] = matrix[i][j];
      else dis[i][j] = INF;
    }
  }
  int MIN = INF;
  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N; i++) {
      for (int j = i + 1; j < N; j++) {
        if (matrix[i][k] > 0 && matrix[k][j] > 0 && MIN > dis[i][j] + matrix[i][k] + matrix[k][j]) {
          MIN = dis[i][j] + matrix[i][k] + matrix[k][j];
          cnt = 0;
          int l = j;
          while (l != i) {
            out[cnt++] = l;
            l = pre[i][l];
          }
          out[cnt++] = i;
          out[cnt++] = k;
        }
      }
    }
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (dis[i][j] > dis[i][k] + dis[k][j]) {
          dis[i][j] = dis[i][k] + dis[k][j];
          pre[i][j] = pre[k][j];
        }
      }
    }
  }
}


void Read(std::vector<std::vector<int> >& v) {
  N = v.size();
  memset(matrix, 0, sizeof(matrix));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
    }
  }
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 4;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][1] = v[1][0] = 1;
  v[0][2] = v[2][0] = 1;
  v[1][2] = v[2][1] = 2;
  v[1][3] = v[3][1] = 2;
  v[2][3] = v[3][2] = 2;
  Read(v);
  MiniLoop();
  for (int i = 0; i < cnt; i++) LOG(INFO) << out[i];
  return 0;
}
