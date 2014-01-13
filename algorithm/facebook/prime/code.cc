// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-08 16:09:32
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 100;
int N;
int matrix[MAXN][MAXN];
int visited[MAXN];
int dis[MAXN];
int pre[MAXN];

int Prime(int s) {
  int rs = 0;
  memset(visited, 0, sizeof(visited));
  memset(pre, 0, sizeof(pre));
  for (int i = 0; i < N; i++) dis[i] = INF;
  dis[s] = 0;
  visited[s] = 1;
  for (int i = 0; i < N; i++) {
    int index = -1;
    int min = INF;
    for (int j = 0; j < N; j++) {
      if (visited[j] == 1 && dis[i] < min) {
        min = dis[i];
        index = i;
      }
    }
    if (index == -1) break;
    rs += min;
    visited[index] = 1;
    for (int j = 0; j < N; j++) {
      if (visited[j] == 0 && matrix[index][j] > 0 && dis[j] > matrix[index][j]) {
        dis[j] = matrix[index][j];
        pre[j] = index;
      }
    }
  }
  return rs;
}

}  // namespace algorithm

namespace algorithm2 {
const int MAXN = 100;
int N;
int matrix[MAXN][MAXN];
int vis[MAXN];
int pre[MAXN];
int root[MAXN];

struct Node {
  int v;
  int u;
  int val;
  Node(int vi, int ui, int vali) : v(vi), u(ui), val(vali){}
};

bool Cmp(const Node & x, const Node & y) {
  return x.val < y.val;
}

int Find(int x) {
  if (root[x] == x) return x;
  return root[x] = Find(root[x]);
}

bool Union(int x, int y)  {
  int px = Find(x);
  int py = Find(y);
  if (px == py) return false;
  else {
    root[px] = py;
    return true;
  }
}

int Krusker() {
  int rs = 0;
  for (int i = 0; i < N; i++) root[i] = i;
  std::vector<Node> vec;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0) {
        vec.push_back(Node(i, j, matrix[i][j]));
      }
    }
  }
  std::sort(vec.begin(), vec.end(), Cmp);
  int num = 0;
  for (int i = 0; i < vec.size(); i++) {
    if (Union(vec[i].u, vec[i].v)) {
      rs += vec[i].val;
      num++;
    }
    if (num == N - 1) break;
  }
  return rs;
}
}  // namespace algorithm2
using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
