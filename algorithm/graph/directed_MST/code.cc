// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: guoliqiang
// I just want to GH to hss~
// Date  : 2013-10-22 02:31:16
// File  : code.cc
// Brief : http://wenku.baidu.com/view/a944f70d79563c1ec4da7103.html
//         朱刘算法
//         zhu_liu 算法
//  对于不固定根的树形图：新加入一个点，和每个点连权相同的边，
//  这个权大于原图所有边权的和，这样此固定根的最小树形图和原图不固定根的最小树形图就对应了。
//
//  有向图求MST，经典的MST是针对无向图的
//

#include "base/public/common_head.h"

namespace WA {  // wrong anwser

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

}  // namespace WA (wrong answer)

namespace algorithm {
const int INF = 0x3f3f3f3f;
const int MAX = 100;

struct Edge {
  int u;
  int v;
  double c;
} E[MAX * MAX];

int to[MAX];
int id[MAX];
int vis[MAX];
double dis[MAX];

double MST(int root, int nv, int ne) {
  double ret = 0;
  while(true) {
    for(int i = 0;i < nv; i++) dis[i] = INF;
    for(int i = 0;i < ne; i++){
      int u = E[i].u;
      int v = E[i].v;
      if(u != v && E[i].c < dis[v]) {
        to[v] = u;
        dis[v] = E[i].c;
      }
    }
    dis[root] = 0;
    // 检查是否连通
    for(int i = 0;i < nv; i++) {
      if(i == root) continue;
      if(dis[i] == INF) return -1;  // 不连通
    }
    // 求环时要求除根节点以外的节点形成的环
    int cnt = 0;
    memset(id, -1, sizeof(id));
    memset(vis, -1, sizeof(vis));
    for(int i = 0;i < nv; i++) {
      ret += dis[i];
      int v = i;
      while(v != root && vis[v] == -1 ) {
        vis[v] = i;
        v = to[v];
      }
      if(v != root && vis[v] == i) {
        for(int u = to[v]; u != v; u = to[u]) id[u] = cnt;
        id[v] = cnt;
        cnt++;
      }
    }

    if(cnt == 0)  break;  // 无环

    for(int i = 0; i < nv; i++)  {
      if(id[i] == -1) id[i] = cnt++;
    }
    //缩点,重新标记
    for(int i = 0;i < ne; i++) {
      int v = E[i].v;
      E[i].u = id[E[i].u];
      E[i].v = id[E[i].v];
      if(E[i].u != E[i].v) E[i].c -= dis[v];
    }
    nv = cnt;
    root = id[root];
  }
  return ret;

}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  /*
  int size = 3;
  std::vector<std::vector<int> > adj(size, std::vector<int>(size, 0));
  adj[0][1] = 10;
  adj[1][2] = 3;
  adj[2][1] = 5;
  if (MSTWrapper(0, adj)) {
    LOG(INFO) << rs;
  }
  */
  return 0;
}
