// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-04 01:32:48
// File  : code.cc
// Brief :
/*
 * online 算法
 * 如果是树的话，可以用中根遍历代替DFS
 * O(E) + O(n*log(n))
 * O(1)
 * 空间复杂度：O(n^2)
 * */
#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int visited[MAX] = {0};
int index[MAX] = {0};
int deep[MAX] = {0};
int e[MAX] = {0};
int id = 0;
int deep2v[MAX] = {0};
int dp[MAX][MAX] = {{0}};

void DFS(int k, int d) {
  index[k] = id;
  e[id] = k;
  deep[id++] = d;
  visited[k] = 1;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] <= 0) continue;
    if (visited[i] == 0) {
      DFS(i, d + 1);
      e[id] = k;
      deep[id++] = d;
    }
  }
}

void DFS() {
  memset(visited, 0, sizeof(visited));
  memset(deep, 0, sizeof(deep));
  memset(index, 0, sizeof(index));
  memset(e, 0, sizeof(e));
  DFS(0, 1);
  for (int i = 0; i < id; i++) {
    LOG(INFO) << e[i] << ":" << deep[i];
  }
}

// RMQ
// dp[i][j]保存的是deep[i]开始2^j个数min值的下表
void Build() {
  for (int i = 0; i < id; i++) {
    // dp[i][0] = deep[i];
    dp[i][0] = i;
  }

  for (int j = 1; (1 << j) < id; j++) {
    int t = 1 << j;
    for (int i = 0; i < id && i + t - 1 < id; i++) {
      int m = 1 << (j - 1);
      if (deep[dp[i][j - 1]] < deep[dp[i + m][j - 1]]) {
        dp[i][j] = dp[i][j - 1];
      } else {
        dp[i][j] = dp[i + m][j - 1];
      }
      // dp[i][j] = std::min(dp[i][j - 1], dp[i + m][j - 1]);
    }
  }
}

int RMQ(int x, int y) {
  double t = y - x + 1;
  int m = log(t) / log(2.0);
  return std::min(dp[x][m], dp[y - (1 << m) + 1][m]);
}

int Get(int x, int y) {
  if (index[x] > index[y]) std::swap(x, y);
  int t = RMQ(index[x], index[y]);
  return e[t];
}

void Read(std::vector<std::vector<int> > & v) {
  N = v.size();
  memset(matrix, 0, sizeof(matrix));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
    }
  }
  DFS();
  Build();
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int  n = 6;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][1] = v[1][0] = 2;
  v[1][3] = v[3][1] = 2;
  v[1][2] = v[2][1] = 2;
  v[0][2] = v[2][0] = 2;
  v[1][4] = v[4][1] = 2;
  v[2][5] = v[5][2] = 2;
  Read(v);
  LOG(INFO) << Get(3, 4);
  return 0;
}
