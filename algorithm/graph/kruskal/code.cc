// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-10 12:53:11
// File  : kruskal.cc
// Brief :

// kruskal算法：构造一个只含n个顶点，而边集为空的子图，若将该子图中各个顶点看成是
// 各棵树的根节点，则它是一个含有n棵树的森林 。之后，从网的边集中选取一条权值最小
// 的边，若该边的两个顶点分属不同的树 ，则将其加入子图，也就是这两个顶点分别所在的
// 两棵树合成一棵树；反之，若该边的两个顶点已落在同一棵树上，则不可取，而应该取下一
// 条权值最小的边再试之。依次类推，直至森林只有一棵树。kruskal算法能够在并查集的基础
// 很快的实现

/*
 * 最小生成森林问题（k颗树）
 * 根据Kruskal算法思想，图中的生成树在连完第n-1条边前，都是一个最小生成森林，每次贪心的选择两个
 * 不属于同一连通分量的树（如果连接一个连通分量，因为不会减少块数，那么就是不合算的）且用最短的
 * 边连起来，连接n-1次后就形成了一棵MST，n-2次就形成了一个两棵树的最小生成森林，n-3，……，n-k此后
 * 就形成了k颗树的最小生成森林，就是题目要求求解的。
 *
 * */

// poj2485

#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int root[MAX] = {0};
int count[MAX] = {0};

struct Node {
  Node(int tu, int tv, int tval) : u(tu), v(tv), val(tval){}
  int u;
  int v;
  int val;
};

bool Cmp(const Node & a, const Node & b) {
  return a.val < b.val;
}

int Root(int k) {
  if (root[k] == k) return k;
  return root[k] = Root(root[k]);
}

bool Merge(int x, int y) {
  int rx = Root(x);
  int ry = Root(y);
  if (rx == ry) return false;
  if (count[rx] < count[ry]) {
    root[rx] = ry;
    count[ry] += count[rx];
  } else {
    root[ry] = rx;
    count[rx] += count[ry];
  }
  return true;
}

int Kruskal(std::vector<std::vector<int> > & v) {
  N = v.size();
  std::vector<Node> foo;
  memset(root, 0, sizeof(root));
  memset(count, 0, sizeof(count));
  for (int i = 0; i < N; i++) {
    root[i] = i;
    count[i] = 1;
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (v[i][j] > 0) foo.push_back(Node(i, j, v[i][j]));
    }
  }
  std::sort(foo.begin(), foo.end(), Cmp);
  int edge_num = 0;
  int rs = 0;
  for (int i = 0; i < foo.size(); i++) {
    if (Merge(foo[i].u, foo[i].v)) {
      edge_num++;
      rs += foo[i].val;
      LOG(INFO) << "add " << foo[i].u << "~" << foo[i].v;
    }
    if (edge_num == N - 1) break;
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 6;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][2] = v[2][0] = 10; 
  v[1][0] = v[0][1] = 3;
  v[1][2] = v[2][1] = 3;
  v[1][3] = v[3][1] = 7;
  v[2][4] = v[4][2] = 8;
  v[3][2] = v[2][3] = 3;
  v[3][5] = v[5][3] = 5;
  v[4][1] = v[1][4] = 8;
  v[5][4] = v[4][5] = 6;
  LOG(INFO) << Kruskal(v);
  return 0;
}
