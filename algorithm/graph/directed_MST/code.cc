// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: guoliqiang
// I just want to GH to hss~
// Date  : 2013-10-22 02:31:16
// File  : code.cc
// Brief : http://wenku.baidu.com/view/a944f70d79563c1ec4da7103.html
//         朱刘算法
//  对于不固定根的树形图：新加入一个点，和每个点连权相同的边，
//  这个权大于原图所有边权的和，这样此固定根的最小树形图和原图不固定根的最小树形图就对应了。

#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int root[MAX] = {0};
int tag[MAX] = {0};
int to[MAX] = {0};
int dis[MAX] = {0};
int rs = 0;

int Root(int k) {
  if (root[k] == k) return k;
  return root[k] = Root(root[k]);
}

void Init() {
  rs = 0;
  for (int i = 0; i < N; i++) root[i] = i;
}

int MST(int r) {
  memset(tag, -1, sizeof(tag));
  memset(to, -1, sizeof(to));
  for (int i = 0; i < N; i++) dis[i] = INT_MAX;
  dis[r] = 0;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      int u = Root(i);
      int v = Root(j);
      if (u == v || matrix[i][j] <= 0) continue;
      if (matrix[i][j] < dis[v]) {  // note: use matrix[i][j] to compare
        to[v] = u;
        dis[v] = matrix[i][j];
      }
    }
  }

  bool circle = false;
  to[r] = -1;
  tag[r] = 0;
  for (int i = 0; i < N; i++) {
    if (Root(i) != i || tag[i] != -1) continue;
    int j = 0;
    for (j = i; j !=  -1 && tag[j] == -1; j = to[j]) tag[j] = 0;
    if (j == -1) return 0; // un_connected.
    if (j == i) {
      circle = true;
      tag[j] = -2;
      LOG(INFO) << "find loop :" << j;
      for (j = to[i]; j != i; j = to[j]) {
        LOG(INFO) << "-->" << j;
        tag[j] = -2;
      }
    }
  }
  if (circle) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        int u = Root(i);
        int v = Root(j);
        if (u != v && tag[v] == -2) matrix[i][j] -= dis[v];  // it is no matter change where tag[u] == -2
      }
    }
    for (int i = 0; i < N; i++) {
      if (tag[i] == -2) {
        rs += dis[i];
        tag[i] = 0;
        for (int j = to[i]; j != i; j = to[j]) {
          rs += dis[j];
          tag[j] = 0;
          root[j] = i;
        }
      }
    }
    if (MST(r) == 0) return 0;  // can happen
  } else {
    for (int i = 0; i < N; i++) {
      if (Root(i) == i) rs += dis[i];
    }
  }
  return 1;
}

int MSTWrapper(int r, std::vector<std::vector<int> > & v) {
  N = v.size();
  memset(matrix, 0, sizeof(matrix));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
    }
  }
  Init();
  return MST(r);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int size = 3;
  std::vector<std::vector<int> > adj(size, std::vector<int>(size, 0));
  adj[0][1] = 10;
  adj[1][2] = 3;
  adj[2][1] = 5;
  if (MSTWrapper(0, adj)) {
    LOG(INFO) << rs;
  }
  return 0;
}
