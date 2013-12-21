// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-21 15:59:12
// File  : code.cc
// Brief :

// http://wenku.baidu.com/link?url=qmhWHRFNgbOj6xtXoKwquiMAJEoYm4tpkPGvQHE1KrRCMINfsdYVXIoze_NtXAbbvZJhRJpk3_ChR9GnqyTFf_rBfyIV220guDVR5Df5II3
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 40;
const int MAXM = 10000;
int ask;
int N;
int M;
int V;

int matrix[MAXN][MAXN];
int A[MAXN][MAXN][4];
int D[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
int dis[MAXN][MAXN];
int in_dis[MAXN][MAXN];

struct Node {
  int v;
  int d;
  int next;
} e[MAXM * 10];
int stop = 0;
int head[MAXM];
int step[MAXM];
int in_step[MAXM];

int SPFA(int s, int tar) {
  memset(step, -1, sizeof(step));
  memset(in_step, 0, sizeof(in_step));
  step[s] = 0;
  in_step[s] = 1;
  std::queue<int> queue;
  queue.push(s);
  while (!queue.empty()) {
    int t = queue.front();
    in_step[t] = 0;
    queue.pop();
    for (int i = head[t]; i != -1; i = e[i].next) {
      int v = e[i].v;
      if (step[v] == -1 || step[v] > step[t] + e[i].d) {
        step[v] = step[t] + e[i].d;
        if (in_step[v] == 0) {
          in_step[v] = 1;
          queue.push(v);
        }
      }
    }
  }
  return step[tar];
}

int SPFA(int sx, int sy, int tx, int ty) {
  memset(dis, -1, sizeof(dis));
  memset(in_dis, 0, sizeof(in_dis));
  
  std::queue<std::pair<int, int> > queue;
  queue.push(std::make_pair(sx, sy));
  dis[sx][sy] = 0;
  in_dis[sx][sy] = 1;
  while (!queue.empty()) {
    std::pair<int, int> t = queue.front();
    queue.pop();
    in_dis[t.first][t.second] = 0;
    for (int i = 0; i < 4; i++) {
      int u = t.first + D[i][0];
      int v = t.second + D[i][1];
      if (matrix[u][v] && (dis[u][v] == -1 || dis[u][v] > dis[t.first][t.second] + 1)) {
        dis[u][v] = dis[t.first][t.second] + 1;
        if (in_dis[u][v] == 0) {
          in_dis[u][v] = 1;
          queue.push(std::make_pair(u, v));
        }
      }
    }
  }
  return dis[tx][ty] == -1 ? INF : dis[tx][ty];
}

void Add(int u, int v, int d) {
  e[++stop].v = v;
  e[stop].d  = d;
  e[stop].next = head[u];
  head[u] = stop;
}

void Build() {
  stop = 0;
  memset(head, -1, sizeof(head));
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= M; j++) {
      for (int k = 0; k < 4; k++) {
        if (matrix[i][j] == 0) continue;
        int u1 = i + D[k][0];
        int v1 = j + D[k][1];
        if (matrix[u1][v1]) {
          Add(A[i][j][k], A[u1][v1][k ^ 1], 1);
          for (int l = 0; l < 4; l++) {
            int u2 = i + D[l][0];
            int v2 = j + D[l][1];
            if (l != k && matrix[u2][v2]) {
              matrix[i][j] = 0;
              int t = SPFA(u1, v1, u2, v2) + 1;
              if (t < INF) Add(A[i][j][k], A[u2][v2][l ^ 1], t);
              matrix[i][j] = 1;
            }
          }
        }
      }
    }
  }
}

void Ask() {
  for (int i = 1; i <= ask; i++) {
    int kx, ky, sx, sy, tx, ty;
    scanf("%d%d%d%d%d%d", &kx, &ky, &sx, &sy, &tx, &ty);
    getchar();
    if (sx == tx && sy == ty) {
      printf("0\n");
      continue;
    }
    if (!matrix[sx][sy] || !matrix[tx][ty]) {
      printf("-1\n");
      continue;
    }
    int S = ++V;
    int T = ++V;
    matrix[sx][sy] = 0;
    for (int i = 0; i < 4; i++) {
      int u = sx + D[i][0];
      int v = sy + D[i][1];
      if (matrix[u][v]) {
        int t = SPFA(kx, ky, u, v);
        if (t < INF) {
          Add(S, A[sx][sy][i], t);
        }
      }
    }
    matrix[sx][sy] = 1;
    for (int i = 0; i < 4; i++) {
      int u = tx + D[i][0];
      int v = ty + D[i][1];
      if (matrix[u][v]) Add(A[tx][ty][i], T, 0);
    }
    int t = SPFA(S, T);
    printf("%d\n", t);
  }
}

void Read() {
  while (scanf("%d%d%d", &N, &M, &ask) != EOF) {
    getchar();
    memset(A, 0, sizeof(A));
    memset(matrix, 0, sizeof(matrix));
    V = 0;
    for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= M; j++) {
        int t = 0;
        scanf("%d", &t);
        matrix[i][j] = t;
        for (int k = 0; k < 4; k++) {
          A[i][j][k] = ++V;
        }
      }
      getchar();
    }
    Build();
    Ask();
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
