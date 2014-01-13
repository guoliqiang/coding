// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-09 22:04:17
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// 最小生成树
namespace algorithm {

const int MAXN = 60;
int matrix[MAXN][MAXN];
int vis[MAXN][MAXN];
int M;
int N;
int cnt = 0;
int bridge = 0;

int matrix2[MAXN * MAXN][MAXN * MAXN];  // bug fixed
int root[MAXN];

struct Node {
  int u;
  int v;
  int val;
  Node (int ui, int vi, int vali) : u(ui), v(vi), val(vali){}
};

void DFS(int x, int y) {
  vis[x][y] = cnt;
  if (x + 1 < M && matrix[x + 1][y] == 1 && vis[x + 1][y] == -1) DFS(x + 1, y);
  if (x - 1 >= 0 && matrix[x - 1][y] == 1 && vis[x - 1][y] == -1) DFS(x - 1, y);
  if (y + 1 < N && matrix[x][y + 1] == 1 && vis[x][y + 1] == -1) DFS(x, y + 1);
  if (y - 1 >= 0 && matrix[x][y - 1] == 1 && vis[x][y - 1] == -1) DFS(x, y - 1);

  if (x + 1 < M && y - 1 >= 0 && matrix[x + 1][y - 1] == 1 && vis[x + 1][y - 1] == -1) DFS(x + 1, y - 1);
  if (x + 1 < M && y + 1 < N && matrix[x + 1][y + 1] == 1 && vis[x + 1][y + 1] == -1) DFS(x + 1, y + 1);
  if (x - 1 >= 0 && y - 1 >= 0 && matrix[x - 1][y - 1] == 1 && vis[x - 1][y - 1] == -1) DFS(x - 1, y - 1);
  if (x - 1 >= 0 && y + 1 < N && matrix[x - 1][y + 1] == 1 && vis[x - 1][y + 1] == -1) DFS(x - 1, y + 1);
}

void DFS() {
  cnt = 0;
  memset(vis, -1, sizeof(vis));
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] == 1 && vis[i][j] == -1) {
        DFS(i, j);
        cnt++;
      }
    }
  }
}

void Build() {
  memset(matrix2, 0, sizeof(matrix2));
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      if (vis[i][j] == -1) continue;
      bool flag = true;

      for (int k = i + 1; k < M && flag; k++) {
        for (int l = -1; l <= 1 && flag; l++) {
          if (j + l < 0 || j + l >= N) continue;
          if (vis[k][j + l] == - 1 || vis[k][j + l] == vis[i][j]) continue;
          int u = vis[i][j];
          int v = vis[k][j + l];
          matrix2[u][v] = matrix2[u][v] == 0 ? k - i - 1 : std::min(k - i - 1, matrix2[u][v]);
          matrix2[v][u] = matrix2[u][v];
          flag = false;
        }
      }

      flag = true;
      for (int k = j + 1; k < N && flag; k++) {
        for (int l = -1; l <= 1 && flag; l++) {
          if (i + l < 0 || i + l >= M) continue;
          if (vis[i + l][k] == - 1 || vis[i + l][k] == vis[i][j]) continue;
          int u = vis[i][j];
          int v = vis[i + l][k];
          matrix2[u][v] = matrix2[u][v] == 0 ? k - j - 1 : std::min(k - j - 1, matrix2[u][v]);
          matrix2[v][u] = matrix2[u][v];
          flag = false;
        }
      }
    }
  }
}

int Find(int k) {
  if (root[k] == k) return root[k];
  return root[k] = Find(root[k]);
}

bool Union(int x, int y) {
  int px = Find(x);
  int py = Find(y);
  if (px == py) return false;
  root[px] = py;
  return true;
}

bool Cmp(const Node & x, const Node & y) {
  return x.val < y.val;
}

int Kruskar() {
  int rs = 0;
  bridge = 0;
  for (int i = 0; i < cnt; i++) root[i] = i;
  std::vector<Node> vec;
  for (int i = 0; i < cnt; i++) {
    for (int j = 0; j < cnt; j++) {
      if (matrix2[i][j] > 0) vec.push_back(Node(i, j, matrix2[i][j]));
    }
  }
  std::sort(vec.begin(), vec.end(), Cmp);
  for (int i = 0; i < vec.size(); i++) {
    int u = vec[i].u;
    int v = vec[i].v;
    if (Union(u, v)) {
      rs += vec[i].val;
      bridge++;
    }
  }
  return rs;
}

void Read() {
  char tmp[100];
  while (scanf("%d%d", &M, &N) != EOF) {
    getchar();
    for (int i = 0; i < M; i++) {
      scanf("%s", tmp);
      for (int j = 0; j < N; j++) {
        if (tmp[j] == '#') matrix[i][j] = 1;
        else matrix[i][j] = 0;
      }
      getchar();
    }
    DFS();
    Build();
    int len = Kruskar();
    printf("%d\n%d %d\n", cnt, bridge, len);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
