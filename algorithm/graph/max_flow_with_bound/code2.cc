// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-22 14:55:43
// File  : code2.cc
// Brief :

// 3.有'源汇'有上下界的最小流
/*    '源点'st，'终点'sd。超级源点ss，超级终点dd。
 *    Note ：这样的网络可能存在往st流的边
 *           也可能存在从sd流出的边
 *           见 305页的例子《图论算法理论实践及应用》
 *           因此最小流可能为负值
 *    首先判断是否存在满足所有边上下界的可行流，方法可以转化成无源汇有上下界的可行流问题。如下：
 *    
 *    增设一条从sd到st没有下界,容量为无穷的边，那么原图就变成了一个无源汇的循环流图。接下来的事情一样，
 *    超级源点ss连i（du[i]>0），i连超级汇点（du[i]<0）,对（ss，dd）进行一次最大流，
 *    当maxflow等于所有(du[]>0)之和时，有可行流，否则没有。
 *
 *    当有可行流时，删除超级源点ss和超级终点dd, 与sd与st增加的边（上下界设为0即可）
 *
 *    以源点为sd，终点为st（注意此时交换了源点与终点）做一次最大流，此时的网络流就是此网络的最小流
 *   
 *
 *   有问题
 *   
 *   而有源汇有上下界最小流问题则是：
 *   1.构造附加网络(不添加[t,s]边)
 *   2.对ss、tt求最大流 (http://www.cnblogs.com/kane0526/archive/2013/04/05/3001108.html 里面有解释为什么先做一次最大流)
 *   3.添加[t,s]边
 *   4.对ss、tt求最大流
 *   5.若ss、tt满流，则[t,s]的流量就是最小流
 *
 *   poj3801
 *
 */
#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int up[MAX][MAX] = {{0}};
int low[MAX][MAX] = {{0}};
int du[MAX] = {0};

int flow[MAX][MAX] = {{0}};

int pre[MAX] = {0};
int visited[MAX] = {0};

void OutFlow(int k = N) {
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < k; j++) {
      std::cout << flow[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void OutMatrix(int k = N) {
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < k; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

int MaxFlow(int source, int target) {
  memset(flow, 0, sizeof(flow));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      flow[i][j] = matrix[i][j];
    }
  }
  int rs = 0;
  while (true) {
    memset(pre, -1, sizeof(pre));
    memset(visited, -1, sizeof(visited));
    std::queue<int> queue;
    queue.push(source);
    visited[source] = 1;
    while (!queue.empty()) {
      int t = queue.front();
      queue.pop();
      if (t == target) break;
      for (int i = 0; i < N; i++) {
        if (visited[i] == -1 && flow[t][i] > 0) {
          visited[i] = 1;
          queue.push(i);
          pre[i] = t;
        }
      }
    }
    if (visited[target] == -1) break;
    int min = INT_MAX;
    for (int i = target; i != source; i = pre[i]) {
      min = std::min(min, flow[pre[i]][i]);
    }
    for (int i = target; i != source; i = pre[i]) {
      flow[pre[i]][i] -= min;
      flow[i][pre[i]] += min;
    }
    if (rs != INT_MAX) {
      if (min == INT_MAX) rs = INT_MAX;
      else rs += min;
    }
  }
  return rs;
}


void MinFlowWithBound(int source, int target, std::vector<std::vector<std::pair<int, int> > > & adj) {
  memset(du, 0, sizeof(du));
  N = adj.size();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      up[i][j] = adj[i][j].first;
      low[i][j] = adj[i][j].second;
      du[j] += low[i][j];
      du[i] -= low[i][j];
    }
  }
  up[target][source] = INT_MAX;
  low[target][source] = 0;

  int ss_out = 0;
  N += 2;
  // N - 2 : ss , N - 1 : st
  for (int i = 0; i < adj.size(); i++) {
    for (int j = 0; j < adj.size(); j++) {
      if (up[i][j] > 0) matrix[i][j] = up[i][j] - low[i][j];
    }
    if (du[i] > 0) matrix[N - 2][i] = du[i];
    if (du[i] < 0) {
      matrix[i][N - 1] = -du[i];
      ss_out += -du[i];
    }
  }
  int max_flow = MaxFlow(N - 2, N - 1);
  if (max_flow != ss_out) {
    LOG(INFO) << "infesiable as max_flow:" << max_flow << " ss_out:" << ss_out;
    return;
  }
  
  N -= 2;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (up[i][j] > 0 && flow[i][j] < matrix[i][j]) {
        flow[i][j] = low[i][j] + (matrix[i][j] - flow[i][j]);
      } else if (up[i][j] > 0) {
        flow[i][j] = low[i][j];
      } else {
        flow[i][j] = 0;
      }
    }
  }
  up[target][source] = 0;
  int cur_flow = 0;
  for (int i = 0; i < N; i++) {
    cur_flow += flow[source][i];
    cur_flow -= flow[i][source];  // 这样的网络可能存在有源点流的边
  }
  
  memset(matrix, 0, sizeof(matrix));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (up[i][j] > 0) {
        matrix[i][j] = up[i][j] - flow[i][j];
        matrix[j][i] = flow[i][j] - low[i][j];
      }
    }
  }
  MaxFlow(target, source);
  int reverse_flow = 0;
  for (int i = 0; i < N; i++) {
    if (matrix[i][source] > 0 && matrix[i][source] > flow[i][source]) {
      reverse_flow += (matrix[i][source] - flow[i][source]);
    }
  }
  LOG(INFO) << "min flow:" << cur_flow - reverse_flow << ", first :" << cur_flow << " reverse:" << reverse_flow;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  int n = 6;
  std::vector<std::vector<std::pair<int, int> > > v;
  v.resize(n, std::vector<std::pair<int, int> >(n, std::make_pair(0, 0)));
  v[0][2].first = 10;
  v[0][2].second = 1;

  v[1][0].first = 3;
  v[1][0].second = 1;

  v[1][3].first = 7;
  v[1][3].second = 5;

  v[2][4].first = 8;
  v[2][4].second = 2;

  v[3][2].first = 3;
  v[3][2].second = 1;

  v[3][5].first = 5;
  v[3][5].second = 3;

  v[4][1].first = 8;
  v[4][1].second = 2;

  v[5][4].first = 6;
  v[5][4].second = 2;
  MinFlowWithBound(0, n - 1, v);
  return 0;
}
