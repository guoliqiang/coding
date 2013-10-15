// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-15 19:37:18
// File  : code.cc
// Brief :
// WA

#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 55;
int N = 0;
int M = 0;
int matrix[MAX][MAX] = {{0}};
int flow[MAX][MAX] = {{0}};
int pre[MAX] = {0};
int visited[MAX] = {0};

int MaxFlow(int source, int target) {
  for (int i = 0; i < 2 *N; i++) {
    for (int j = 0; j < 2 * N; j++) {
      flow[i][j] = matrix[i][j];
    }
  }
  int rs = 0;
  while (true) {
    memset(pre, 0, sizeof(pre));
    memset(visited, 0, sizeof(visited));
    std::queue<int> queue;
    queue.push(source);
    visited[source] = 1;
    while (!queue.empty()) {
      int t = queue.front();
      queue.pop();
      if (t == target) break;
      for (int i = 0; i < 2 * N; i++) {
        if (visited[i] == 0 && flow[t][i] > 0) {
          visited[i] = 1;
          pre[i] = t;
          queue.push(i);
        }
      }
    }
    if (visited[target] == 0) break;
    int min = INT_MAX;
    for (int i = target; i != source; i = pre[i]) {
      min = std::min(min, flow[pre[i]][i]);
    }
    for (int i = target; i != source; i = pre[i]) {
      flow[pre[i]][i] -= min;
      flow[i][pre[i]] += min;
    }
    if (rs != INT_MAX) {
      if (min == INT_MAX) rs = INT_MAX;
      else rs += min;
    }
  }
  return rs;
}

void ReadFromConsole() {
  freopen("test.txt", "r", stdin);
  while(scanf("%d %d", &N, &M) != EOF) {
    // LOG(INFO) << "N:" << N << " M :" << M;
    if (N == 1 && M == 0) {
      printf("1\n");
      continue;
    }
    if (M == 0) {
      printf("0\n");
      continue;
    }
    memset(matrix, 0, sizeof(matrix));
    int source = -1;
    for (int i = 0; i < M; i++) {
      while (getchar() != '(');
      int x = 0;
      int y = 0;
      scanf("%d,%d", &x, &y);
      matrix[x][N + x] = 1;
      matrix[y][N + y] = 1;
      matrix[N + x][y] = INT_MAX;
      matrix[N + y][x] = INT_MAX;
      if (source == -1) source = N + x;
    }
    while (getchar() != '\n');
    int min = INT_MAX;
    for (int i = 0; i < N; i++) {
      if (i + N == source) continue;
      int t = MaxFlow(source, i);
      // LOG(INFO) << source << "~" << i << ":" << t;
      min = std::min(min, t);
    }
    if (min == INT_MAX) {
      printf("%d\n", N);
    } else {
      printf("%d\n", min);
    }
  }
}

// 并求出点割集合
// 逐一删除某个点形成的边，最大流变小，次点为割点
// http://yzmduncan.iteye.com/blog/1163170
void ReadFromConsole2() {
  freopen("test.txt", "r", stdin);
  while(scanf("%d %d", &N, &M) != EOF) {
    if (N == 1 && M == 0) {
      printf("1\n");
      continue;
    }
    if (M == 0) {
      printf("0\n");
      continue;
    }
    memset(matrix, 0, sizeof(matrix));
    int source = -1;
    for (int i = 0; i < M; i++) {
      while (getchar() != '(');
      int x = 0;
      int y = 0;
      scanf("%d,%d", &x, &y);
      matrix[x][N + x] = 1;
      matrix[y][N + y] = 1;
      matrix[N + x][y] = INT_MAX;
      matrix[N + y][x] = INT_MAX;
      if (source == -1) source = N + x;
    }
    while (getchar() != '\n');
    int min = INT_MAX;
    int min_t = -1;
    for (int i = 0; i < N; i++) {
      if (i + N == source) continue;
      int t = MaxFlow(source, i);
      if (t < min) {
        min_t = t;
        min = t;
      }
    }
    if (min == INT_MAX) {
      printf("%d\n", N);
    } else {
      printf("%d\n", min);
      for (int k = 0; k < N && min; k++) {
        if (k == min_t || k == source - N) continue;
        if (matrix[k][N + k] == 1) {
          matrix[k][N + k] = 0;
          int tmp = MaxFlow(source, min_t);
          if (tmp < min) {
            LOG(INFO) << "cut point " << k;
            min--;
          } else {
            matrix[k][N + k] = 1;
          }
        }
      }
    }
  }
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  ReadFromConsole();
  return 0;
}
