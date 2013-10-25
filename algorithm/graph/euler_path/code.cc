// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-12 17:14:48
// File  : code.cc
// Brief :
// http://www.cppblog.com/abilitytao/archive/2010/07/26/121319.html
/*
 * 欧拉回路：每条边恰好只走一次，并能回到出发点的路径
 * 欧拉路径：经过每一条边一次，但是不要求回到起始点
 *
 * 首先看欧拉回路存在性的判定：
 * 一、无向图
 * 每个顶点的度数都是偶数，则存在欧拉回路。
 *
 * 二、有向图（所有边都是单向的）
 * 每个节顶点的入度都等于出度，则存在欧拉回路。
 *
 * 三.混合图欧拉回路 (现实中这种图是最多的，比如公交网，大多数路是双行道，一小部分是单行路)
 * 混合图欧拉回路用的是网络流。
 * 把该图的无向边随便定向，计算每个点的入度和出度。如果有某个点出入度之差为奇数，那么肯定不存在欧拉回路。
 * 因为欧拉回路要求每点入度 = 出度，也就是总度数为偶数，存在奇数度点必不能有欧拉回路。
 * 好了，现在每个点入度和出度之差均为偶数。那么将这个偶数除以2，得x。也就是说，对于每一个点，只要将x条边
 * 改变方向（入度 > 出度 就是变入，出度 > 入度 就是变出），就能保证出 = 入。如果每个点都是出 = 入，那么
 * 很明显，该图就存在欧拉回路。
 * 现在的问题就变成了：我该改变哪些边，可以让每个点出 = 入？构造网络流模型。首先，有向边是不能改变方向的，
 * 要之无用，删。一开始不是把无向边定向了吗？定的是什么向，就把网络构建成什么样，边长容量上限1。另新建s(源点)和t(目的点)。
 * 对于入 > 出的点u，连接边(u, t)、容量为x，对于出 > 入的点v，连接边(s, v)，容量为x（注意对不同的点x不同）。之后，察看是
 * 否有满流的分配。有就是能有欧拉回路，没有就是没有。欧拉回路是哪个？查看流值分配，将所有流量非 0（上陂那么，没和s、t连接
 * 的点怎么办？和s连接的条件是出 > 入，和t连接的条件是入 > 出，那么这个既没和s也没和t连接的点，自然早在开始就已经满足入 = 出了。
 * 那么在网络流过程中，这些点属于“中间点”。我们知道中间点流量不允许有累积的，这样，进去多少就出来多少
 * 
 * .欧拉路径存在性的判定
 *
 * 一无向图
 * 一个无向图存在欧拉路径，当且仅当该图所有顶点 所有点的度数全为偶数　或者　除了两个度数为奇数外其余的全是偶数。
 *
 * 二有向图
 * 一个有向图存在欧拉路径，当且仅当该图所有顶点的入度等于出度,  或者
 * 除了两个顶点, 其中一个顶点的入度 = 出度 +１，另一个 出度＝入度＋１，其余点入度等于出度。
 *
 * 三。混合图欧拉路径
 * 求欧拉路径的第一步一定是求欧拉回路，在混合图上也不例外，如何判断混合图欧拉回路问题的存在性呢？
 * 首先，我们用上文所说的方法判断该图是否存在欧拉回路，如果存在，欧拉路径一定存在。如果欧拉回路不存在，
 * 那么我们枚举欧拉路径的起点和终点，连接一条无向边，然后再用最大流判断是否存在欧拉回路即可。
 *
 * */

#include "base/public/common_head.h"
#include "../base/graph.h"

namespace algorithm {

bool HasEulerSinglePath(std::vector<std::vector<int> > & matrix, bool directed = true) {
  int n = matrix.size();
  if (directed) {
    int bad = 0;
    for (int i = 0; i < n; i++) {
      int come = 0;
      int out = 0;
      for (int j = 0; j < n; j++) {
        if (matrix[i][j] > 0) out++;
        if (matrix[j][i] > 0) come++;
      }
      if (abs(out - come) == 0) continue;
      if (abs(out - come) == 1) bad++;
      else return false;
    }
    if (bad != 2 && bad != 0) return false;
  } else {
    int bad = 0;
    for (int i = 0; i < n; i++) {
      int degree = 0;
      for (int j = 0; j < n; j++) {
        if (matrix[i][j] > 0) degree++;
      }
      if (degree % 2) bad++;
    }
  }
  return true;

}

bool HasEulerLoopPath(std::vector<std::vector<int> > & matrix, bool directed = true) {
  int n = matrix.size();
  if (directed) {
    for (int i = 0; i < n; i++) {
      int come = 0;
      int out = 0;
      for (int j = 0; j < n; j++) {
        if (matrix[i][j] > 0) out++;
        if (matrix[j][i] > 0) come++;
      }
      if (out != come) return false;
    }
  } else {
    for (int i = 0; i < n; i++) {
      int degree = 0;
      for (int j = 0; j < n; j++) {
        if (matrix[i][j] > 0) degree++;
      }
      if (degree % 2) return false;
    }
  }
  return true;
}

int EdgeNum(std::vector<std::vector<int> > & matrix, bool directed = true) {
  int rs = 0;
  int n = matrix.size();
  if (directed) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (matrix[i][j] > 0) rs++;
      }
    }
  } else {
    for (int i = 0; i < n; i++) {
      for (int j = i; j < n; j++) {
        if (matrix[i][j] > 0) rs++;
      }
    }
  }
  return rs;
}

void EulerLoopPath(std::vector<std::vector<int> > & matrix,
                   std::vector<int> & path, 
                   int begin = 0,
                   bool directed = true) {
  int n = matrix.size();
  std::vector<std::vector<bool> > visited(n, std::vector<bool>(n, false));
  int cur = begin;
  path.push_back(cur);
  int edge_num = EdgeNum(matrix, directed);
  for (int i = 0; i < edge_num; i++) {
    for (int j = 0; j < n; j++) {
      if (matrix[cur][j] > 0 && visited[cur][j] == false)  {
        path.push_back(j);
        visited[cur][j] = true;
        cur = j;
        if (!directed) visited[j][cur] = true;
      }
    }
  }
}

void DFS(std::vector<std::vector<int> > & matrix,
         std::vector<std::vector<bool> > & visited,
         std::stack<int> & stack,
         bool directed = true) {
  int t = stack.top();
  int n = matrix.size();
  for (int i = 0; i < n; i++) {
    if (matrix[t][i] > 0 && visited[t][i] == false) {
       stack.push(i);
       visited[t][i] = true;
       if (!directed) visited[i][t] = false;
       DFS(matrix, visited, stack);
    }
  }
}

// 弗罗莱(Fleury)算法
// http://blog.163.com/zhoumhan_0351/blog/static/39954227200982051154725/
void EulerSinglePath(std::vector<std::vector<int> > & matrix,
                     std::vector<int> & path, bool directed = true) {
  int n = matrix.size();
  int b = -1;
  int e = -1;
  if (directed) {
    for (int i = 0; i < n; i++) {
      int come = 0;
      int out = 0;
      for (int j = 0; j < n; j++) {
        if (matrix[i][j] > 0) out++;
        if (matrix[j][i] > 0) come++;
      }
      if (come + 1 == out) b = i;
      if (out + 1 == come) e = i;
    }
  } else {
    for (int i = 0; i < n; i++) {
      int degree = 0;
      for (int j = 0; j < n; j++) {
        if (matrix[i][j] > 0) degree++;
      }
      if (degree % 2 == 1 && b == -1) b = i;
      if (degree % 2 == 1 && e == -1) e = i;
    }
  }

  if (b == -1) b = 0;  // 存在欧拉回路任意点作为起点都可以
  std::vector<std::vector<bool> > visited(n, std::vector<bool>(n, false));
  std::stack<int> stack;
  stack.push(b);
  while (!stack.empty()) {
    int t = stack.top();
    int bridge = 1;
    for (int i = 0 ; i < n; i++) {
      if (matrix[t][i] > 0 && visited[t][i] == false) {
        bridge = 0;
        break;
      }
    }
    if (bridge == 0) {
      DFS(matrix, visited, stack, directed);
    } else {
      path.push_back(stack.top());
      stack.pop();
    }
  }
}

}//  namespace algorithm


using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix;
  AdjaMatrix(matrix, 5);
  SetMatrix(matrix, 0, 1, 2);
  SetMatrix(matrix, 2, 0, 5);
  // SetMatrix(matrix, 0, 3, 1);
  SetMatrix(matrix, 1, 4, 3);
  SetMatrix(matrix, 4, 2, 4);
  LOG(INFO) << HasEulerLoopPath(matrix);
  LOG(INFO) << HasEulerSinglePath(matrix);
  if (HasEulerSinglePath(matrix)) {
    std::vector<int> rs;
    EulerSinglePath(matrix, rs);
    LOG(INFO) << JoinVector(rs);
  }
  if (HasEulerLoopPath(matrix)) {
    std::vector<int> rs;
    EulerLoopPath(matrix, rs);
    LOG(INFO) << JoinVector(rs);
  }
  return 0;
}
