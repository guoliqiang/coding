// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-12 23:46:46
// File  : code.cc
// Brief :


#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1003;
int N;
int matrix[MAXN][MAXN];
int result[MAXN];
int dfsn[MAXN];
int visited[MAXN];
int low[MAXN];
int root = 0;
int num = 0;
int cnt = 0;

void DFS(int k, int father) {
  dfsn[k] = low[k] = cnt++;
  int count = 0;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (dfsn[i] == -1) {
        count++;
        DFS(i, k);
        low[k] = std::min(low[k], low[i]);
        if ((k == root && count >= 2) ||
            (k != root && low[i] >= dfsn[k])) {
          // LOG(INFO) << "k:" << k << " root:" << root
          //           << "low[" << i << "]:" << low[i]
          //           << " low[" << k << "]:" << low[k];
          result[k] = 1;
        }
      } else {
        if (i != father) {
          // 两种比较方式都可以
          // low[k] = std::min(low[k], dfsn[i]);
          low[k] = std::min(low[k], low[i]);
        }
      }
    }
  }
}

void DFS() {
  memset(dfsn, -1, sizeof(dfsn));
  memset(low, -1, sizeof(low));
  DFS(root, -1);
}

void DFS(int k) {
  visited[k] = 1;
  for (int i = 0; i < N; i++) {
    if (visited[i] == 0 && matrix[k][i] > 0) DFS(i);
  }
}



void Read() {
  while (true) {
    num++;
    memset(matrix, 0, sizeof(matrix));
    memset(result, 0, sizeof(result));
    int u = 0;
    scanf("%d", &u);
    if (u == 0) break;
    while (u != 0) {
      int v = 0;
      scanf("%d", &v);
      matrix[u - 1][v - 1] = 1;
      matrix[v - 1][u - 1] = 1;
      root = u - 1;
      N = std::max(N, std::max(u, v));
      scanf("%d", &u);
    }
    DFS();
    printf("Network #%d\n", num);
    bool find = false;
    for (int i = 0; i < N; i++) {
      if (result[i] == 1) {
        find = true;
        memset(visited, 0, sizeof(visited));
        visited[i] = 1;
        int rs = 0;
        for (int j = 0; j < N; j++) {
          if (matrix[i][j] > 0 && visited[j] == 0) {
            DFS(j);
            rs++;
          }
        }
        printf("  SPF node %d leaves %d subnets\n", i + 1, rs);
      }
    }
    if (!find) printf("  No SPF nodes\n");
    printf("\n");
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
