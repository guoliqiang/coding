// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-12 23:08:25
// File  : code.cc
// Brief :

/*
 * 图找割点，有向图和无向图的差别是，无向图递归时不要走到父节点的位置
 * http://www.cppblog.com/Icyflame/archive/2009/07/04/89231.html
 *
 * 求割顶(点)
 *
 * 割顶是去掉后让图不再连通的点。
 * 求割顶的算法在DFS遍历的算法上形成。
 * 
 * 什么样的点是割顶？
 * 在一棵DFS树中，
 * 1.根root是割顶   -------------  它至少有两个'儿子'
 * 2.其他点v是割顶  -------------  它有一个儿子u, 从u或者u的后代出发没有指向v祖先(不含v)的B边, 则删除v以后u和v的父亲不连通, 故为割顶。
 *
 *
 * tarjan 深度优先搜索是神器
 * http://www.cnblogs.com/rainydays/archive/2011/07/06/2099516.html
 * 定理，当把边的双连通分支缩点后形成了一个有向无环图，叶子（度为1的点）的个数为n，则需要在原图中添加(n + 1)/2条边，可以使原图变为没有桥的双连通图。
 * 怎么证明这个结论呢?
 * 具体办法为，首先在两个距离公共祖先最远的两个叶节点之间连接一条边，把这两个点
 * 到祖先的路径上所有点紧缩到成为一个环，因为一个形成的环必然是双连通的。然后再
 * 找两个距离公共祖先最远的两个叶节点，由此类推,一对一对找完，正好是(n+1)/2次，
 * 把所有点紧缩到了一路。
 *
 * */
#include "base/public/common_head.h"

namespace algorithm {
const int MAXN = 100;
int N = 0;
int matrix[MAXN][MAXN];
int dfsn[MAXN] = {-1};
int low[MAXN] = {-1};
int flag[MAXN] = {false};
int num = 0;
int root = 0;

// 有向图
void DFS(int k) {
  dfsn[k] = low[k] = num++;
  int count = 0;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (dfsn[i] == -1) {
        count++;
        DFS(i);
        low[k] = std::min(low[k], low[i]);
        if ((k == root && count >= 2) ||
            (k != root && low[i] >= dfsn[k])) flag[k] = true;  // 注意要和dfsn[k]比较非low[k]
            /* 注意和求割边不同，这里是有等号的,表示如下情况
                1
                |
                |
                2
             /   \
            3 -- 4
            2是割点
          */
      } else {
        // 两种比较方式都可以
        // low[k] = std::min(low[k], dfsn[i]);
        low[k] = std::min(low[k], low[i]);
      }
    }
  }
}

// 无向图
void DFS(int k, int father) {
  dfsn[k] = low[k] = num++;
  int count = 0;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (dfsn[i] == -1) {
        count++;
        DFS(i, k);
        low[k] = std::min(low[k], low[i]);
        if ((k == root && count >= 2) ||
            (k != root && low[i] >= dfsn[k])) flag[k] = true;
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
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
