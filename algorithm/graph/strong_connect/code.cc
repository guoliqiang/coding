// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-24 04:32:01
// File  : code.cc
// Brief :
// 强连通（Strongly Connected）是指有向图G（Directed Graph）中任意两点v1、v2之间都存在
// 着v1到v2的路径(path,若途径的点和边都不重复,则称为路径)及v2到v1的路

// http://en.wikipedia.org/wiki/Depth-first_search#Output_of_a_depth-first_search
// http://www.dlifep.com/?p=825
// 树边 ：
// 后向边 ： 
// 前向边 ：
// 横叉边 :

// 如果有向图是强连通的，则任两个节点都是相互可达。故必可做一回路经过图中所有各点。若不然则必有一
// 回路不包含某一结点v，并且v与回路上的个节点就不是相互可达，与强连通条件矛盾
//
// http://blog.csdn.net/geniusluzh/article/details/6601514
// http://wenku.baidu.com/view/0bafd729ed630b1c59eeb54c.html
// Tarjan算法是基于这样一个原理：如果u是某个强连通分量的根，那么：
//
// （1）u不存在路径可以返回到它的祖先
// （2）u的子树也不存在路径可以返回到u的祖先。
//
// 实现Tarjan算法的时候，使用dfn[i]记录节点i被访问的时间，也可以理解为在访问该点之前已经访问的点的个数。
// 然后使用数组low[i]记录点i或者i的子树最小可以返回到的节点（在栈中）的次序号。
//
// low[i]的更新过程:
//
//   if(v是i向下dfs的树边) low[i]=min(low[i],low[v]);//这里也就是说low[i]表示i或者i的子树所能追回到的最小的点序号。
//   if(v不是树边也不是横叉边) low[i]=min(low[i],dfsn[v]); //其实这里你直接更新成low[v]代替dfs[v]也是可以的
//
//   根据上面的原理，我们可以发现只有当dfsn[i]==low[i]的时候就正好是强连通分量的根。
//   这个时候我们把在栈中的点（在遇到根之前在栈中的点）出栈，并且标记好点所属的强连通分支的编号。
//
//   整个Tarjan算法跑下来就可以完成强连通分支的求解了。

#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int low[MAX] = {0};
int dfn[MAX] = {0};
int stack[MAX] = {0};
int id[MAX] = {0};
int stack_ptr = 0;
int tag = 0;
int cnt = 0;

void Tarjan(int k) {
  stack[stack_ptr++] = k;
  low[k] = dfn[k] = cnt++;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (!dfn[i]) {
        Tarjan(i);
        if (low[i] < low[k]) low[k] = low[i];
      } else {
        if (dfn[i] < dfn[k]) {  // 向后边
          int j = 0;
          while (j < stack_ptr && stack[j] != i) j++;  // 如果i是横叉边的端点，其不会出现在stack中
          if (j < stack_ptr && dfn[i] < low[k]) low[k] = dfn[i];  // 可能有多条向后边，因此需要判断dfb[i] < low[k]
        }
      }
    }
  }
  if (low[k] == dfn[k]) {
    LOG(INFO) << "component " << tag << ":";
    while (stack[stack_ptr] != k) {
      stack_ptr--;
      id[stack[stack_ptr]] = tag;
      LOG(INFO) << stack[stack_ptr];
    }
    tag++;
  }
}

int Find() {
  stack_ptr = 0;
  cnt = 0;
  memset(dfn, 0, sizeof(dfn));
  for (int i = 0; i < N; i++) {
    if (!dfn[i]) Tarjan(i);
  }
  return tag;
}

void Read(std::vector<std::vector<int> > & v) {
  N = v.size();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 6;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][2] = 1;
  v[0][1] = 1;
  v[1][3] = 1;
  v[2][3] = 1;
  v[3][0] = 1;
  v[2][4] = 1;
  v[3][5] = 1;
  v[4][5] = 1;
  Read(v);
  Find();
  return 0;
}
