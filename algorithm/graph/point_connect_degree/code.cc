// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-15 17:18:59
// File  : code.cc
// poj1966
// Brief : http://www.cnblogs.com/dyllove98/archive/2013/07/25/3215003.html
//         http://www.cnblogs.com/-hsz/archive/2012/07/24/2607375.html
//
//
// 通俗点说，就是一个图G最少要去掉多少个点会变成非连通图或者平凡图。
// 当然对于一个完全图来说Kn来说，它的连通度就是n-1(poj1966要返回n)。
//
// 平凡图：只含有一个独立点的图
//
// 点连通度：
//　　求一个给定的无向图的点连通度，可以转换为求边连通度，怎么转换就如下所示：
// 
// 将点连通度转化为边连通度时怎么建图呢：
// 1 ．构造一个网络 N
//  若 G 为无向图：
//   (1) 原 G 图中的每个顶点 v 变成 N 网中的两个顶点 v' 和 v" ，顶点 v' 至 v" 有一条弧（有向边）连接，弧容量为 1;
//   (2) 原 G 图中的每条边 e ＝ uv ，在 N 网中有两条弧 e’= u"v',e"=v"u' 与之对应， e' 弧容量为 ∞ ， e" 弧容量为 ∞
//   (3)A” 为源顶点， B' 为汇顶点
//    注意：弧是有向边
//    u'--->u''--->v'--->v''
//   /|\                 |
//    |__________________|
//
//  若 G 为有向图：
//    (1) 原 G 图中的每个顶点变成 N 网中的两个顶点 v’ 和 v” ，顶点 v' 至 v” 有一条弧连接，弧容量为 1
//    (2) 原 G 图中的每条弧 e ＝ uv 变成一条有向轨 u'u"v'v" ，其中轨上的弧 u"v' 的容量为 ∞;
//    (3)A” 为源顶点， B' 为汇顶点
//    
//    u'-->u''--->v'--->v''
//
// 2 ．指定一个源点 A" ，枚举汇点B'，求 A" 到 B' 的最大流 F ，
//   　注意　Ａ'' B' 是说他们来自于不同的两个点

/*
 * 另外一个问题，一个图添加多少边后变的双向连通，见find_edge_point
 * */

#include "base/public/common_head.h"
#include <stdio.h>
namespace algorithm {
const int MAX = 1000;
int n;
int pre[MAX] = {0};
int visited[MAX] = {0};
int flow[MAX][MAX];
int matrix[MAX][MAX];
int cut[MAX] = {0};

int MaxFlow(int source, int target) {
  int rs = 0;
  for (int i = 0; i < n * 2; i++) {
    for (int j = 0; j < n * 2; j++) {
      flow[i][j] = matrix[i][j];
    }
  }
  while (true) {
    memset(pre, 0, sizeof(pre));
    memset(visited, 0, sizeof(visited));
    std::queue<int> queue;
    queue.push(source);
    visited[source] = 1;
    while (!queue.empty()) {
      int t = queue.front();
      queue.pop();
      if (visited[target] == 1) break;
      for (int i = 0; i < 2 * n; i++) {
        if (visited[i] == 0 && flow[t][i] > 0) {
          queue.push(i);
          pre[i] = t;
          visited[i] = 1;
        }
      }
    }
    if (visited[target] == 0) break;
    int min = INT_MAX;
    for (int i = target; i != source; i = pre[i]) {
      min = std::min(min, flow[pre[i]][i]);
    }
    for (int i = target; i != source; i = pre[i]) {
      flow[pre[i]][i] -= min;
      flow[i][pre[i]] += min;
    }
    // LOG(INFO) << "min:" << min << " rs :" << rs;
    // 防止越界
    if (rs != INT_MAX) {
      if (min == INT_MAX) rs = min;
      else rs += min;
    }
   
  }
  return rs;
}

int UnDirected(std::vector<std::vector<int> > & v) {
  memset(matrix, 0, sizeof(matrix));
  memset(flow, 0, sizeof(flow));

  n = v.size();
  int k = -1;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (v[i][j] > 0) {
        matrix[i][n + i] = 1;
        matrix[j][n + j] = 1;
        matrix[n + i][j] = INT_MAX;
        matrix[n + j][i] = INT_MAX;
        if (k == -1) k = n + i;
      }
    }
  }
  int rs = INT_MAX;
  int target = -1;
  for (int i = 0; i < n; i++) {
    if (i + n == k) continue;  // 保证是不同的两个点
    int t = MaxFlow(k, i);
    if (t < rs) {
      rs = t;
      target = i;
    }
  }
  if (rs == INT_MAX) {// 强连通图，需要去掉所有的点
    return n;
  } else {
    memset(cut, 0, sizeof(cut));
    int flow_value = rs;
    // 此时ｓｔ一定不直接相邻，因为如果ｓｔ直接相邻rs = INT_MAX
    // todo poj1815
    for (int i = 0; i < n && flow_value; i++) {
      if (i == target || i == k - n) continue;  // 尝试删除处ｓｔ点外其它的每个点，因为最终要求的就是这种点有几个(想象一下原图)
      matrix[i][n + i] = 0;
      if (MaxFlow(k, target) < flow_value) {
        cut[i] = 1;
        flow_value--;
      } else {
        matrix[i][n + i] = 1;
      }
    }
    return rs;
  }
}

int Directed(std::vector<std::vector<int> > & v) {
  memset(matrix, 0, sizeof(matrix));
  memset(flow, 0, sizeof(flow));

  n = v.size();
  int k = -1;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; i < n; i++) {
      if (v[i][j] > 0) {
        matrix[i][n + i] = 1;
        matrix[j][n + j] = 1;
        matrix[n + i][j] = INT_MAX;  // Note here
        if (k == -1) k = n + i;
      }
    }
  }
  int rs = INT_MAX;
  int target = -1;
  for (int i = 0; i < n; i++) {
    if (i + n == k) continue;
    int t = MaxFlow(k, i);
    if (t < rs) {
      rs = t;
      target = i;
    }
  }
  if (rs == INT_MAX) { // 强连通图，需要去掉所有的点
    return n;
  } else {
    memset(cut, 0, sizeof(cut));
    int flow_value = rs;
    for (int i = 0; i < n && flow_value; i++) {
      if (i == target || i == k - n) continue;  // 跳过源点和汇点，如果源点和汇点直接相连，rs == INT_MAX 在上面就已经返回了
      matrix[i][n + i] = 0;
      if (MaxFlow(k, target) < flow_value) {
        cut[i] = 1;
        flow_value--;
      } else {
        matrix[i][n + i] = 1;
      }
    }
    return rs;
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > v(3, std::vector<int>(3, 1));
  LOG(INFO) << UnDirected(v);
  return 0;
}
