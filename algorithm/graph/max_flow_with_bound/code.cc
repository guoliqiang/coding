// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-17 17:12:25
// File  : code.cc
// Brief :
// http://wenku.baidu.com/view/0f3b691c59eef8c75fbfb35c.html
// http://www.cnblogs.com/kane0526/archive/2013/04/05/3001108.html

/*
 * 1.无源汇有上下界最大流
 *   增设一个超级源点ss和一个超级终点dd
 *   du[i]=in[i]（i节点所有入流下界之和）-out[i]（i节点所有出流下界之和）。
 *
 *   当du[i]大于0的时候，st到i连一条流量为du[i]的边。
 *   // 在所有边都是下界流量时，i必须有du[i]的流进入，才能平衡
 *
 *   当du[i]小于0的时候，i到sd连一条流量为-du[i]的边。
 *   // 在所有边都是下界流量时，i必须有-du[i]的流出入，才能平衡 (-du[i] 是正数)
 *
 *   最后对（ss，dd）求一次最大流即可，当所有附加边全部满流时（即maxflow==所有du[]>0之和），有可行解。
 *   即st与sd相连的边全部是满流
 *  
 * 2. 有源汇有上下界的最大流
 *    源点st，终点sd。超级源点ss，超级终点dd。
 *    首先判断是否存在满足所有边上下界的可行流，方法可以转化成无源汇有上下界的可行流问题。如下：
 *    
 *    增设一条从d到s没有下界容量为无穷的边，那么原图就变成了一个无源汇的循环流图。接下来的事情一样，
 *    超级源点ss连i（du[i]>0），i连超级汇点（du[i]<0）,对（ss，dd）进行一次最大流，
 *    当maxflow等于所有(du[]>0)之和时，有可行流，否则没有。
 *
 *    当有可行流时，删除超级源点ss和超级终点dd，再对（s，d）进行一次最大流，此时得到的maxflow则为题目的解。
 * */

#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
int N = 0;
int up[MAX][MAX] = {{0}};
int low[MAX][MAX] = {{0}};
int matrix[MAX][MAX] = {{0}};

int sum_in[MAX] = {0};
int sum_out[MAX] = {0};

int flow[MAX][MAX] = {{0}};
int pre[MAX] = {0};
int visited[MAX] = {0};

void OutMatrix(int n = N) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void OutFlow(int n = N) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cout << flow[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

int MaxFlow(int source, int target) {
  // LOG(INFO) << "source:" << source << " target:" << target;
  int rs = 0;
  memset(flow, 0, sizeof(flow));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
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
      if (t == target) break;
      for (int i = 0; i < N; i++) {
        if (visited[i] == 0 && flow[t][i] > 0) {
          visited[i] = 1;
          queue.push(i);
          pre[i] = t;
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
    // LOG(INFO) << "cur flow:" << min;
    if (rs != INT_MAX) {
      if (min == INT_MAX) rs = INT_MAX;
      else rs += min;
    }
  }
  return rs;
}

void MaxFlowWithBound(std::vector<std::vector<std::pair<int, int> > > & adj, int source, int target) {
  N = adj.size() + 2;
  memset(matrix, 0, sizeof(up));
  memset(low, 0, sizeof(low));
  memset(sum_in, 0, sizeof(sum_in));
  memset(sum_out, 0, sizeof(sum_out));
  // 第一次最大流假设网络中已经存在有各个边下界的流
  // 因此每条边流的的自由度为up - low
  for (int i = 0; i < adj.size(); i++) {
    for (int j = 0; j < adj.size(); j++) {
      up[i][j] = adj[i][j].first;
      low[i][j] = adj[i][j].second;
      sum_in[j] += low[i][j];
      sum_out[i] += low[i][j];
      matrix[i][j] = up[i][j] - low[i][j];
    }
  }
  // 链接sd -- > st 组成循环网络
  low[target][source] = 0;
  up[target][source] = INT_MAX;
  matrix[target][source] = INT_MAX;
  // ss = N - 2  tt == N - 1
  int ss_out = 0;
  for (int i = 0; i < adj.size(); i++) {
    int t = sum_in[i] - sum_out[i];
    if (t > 0) matrix[N - 2][i] = t;  // ss -> i
    if (t < 0) {
      matrix[i][N - 1] = -t;  // i -> dd
      ss_out += -t;
    }
  }
  int max_flow = MaxFlow(N - 2, N - 1);
  if (max_flow != ss_out)  {
    LOG(INFO) << "infesiable";
    return;
  }
  up[target][source] = 0;  // 也可以不改
  N -= 2;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (up[i][j] > 0 && flow[i][j] < matrix[i][j]) {
        // 第一次最大流之后有流从这条边流过
        flow[i][j] = low[i][j] + (matrix[i][j] - flow[i][j]);
      } else if (up[i][j] > 0) {
        flow[i][j] = low[i][j];  // 第一次最大流之后有流从j->i边流过
      } else {
        flow[i][j] = 0;
      }
    }
  }
  // 求的当前网络中的流值
  // 第一次最大流的结果 >= 这个值，因为可能存在ss -> sd -> st -> dd 流
  max_flow = 0;
  for (int i = 0; i < N; i++) max_flow += flow[source][i];
  
  memset(matrix, 0, sizeof(matrix));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (up[i][j] > 0) {
        matrix[i][j] = up[i][j] - flow[i][j];
        matrix[j][i] = flow[i][j] - low[i][j];
        // 避免第二次最大流后反向边的流导致结果小于low
        // 因此反向边的自由度设为flow - low,而不能是flow
      }
    }
  }
  max_flow += MaxFlow(source, target);
  LOG(INFO) << "max flow:" << max_flow;

}

}  // nameapace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int size = 6;
  std::vector<std::vector<std::pair<int, int> > > adj(size, std::vector<std::pair<int, int> >(size, std::make_pair(0, 0)));
  /*
  adj[0][1].first = 7;
  adj[0][1].second = 4;
  */
  /*
  adj[0][2].first = 3;
  adj[0][2].second = 2;
  
  adj[1][2].first = 4;
  adj[1][2].second = 2;
  
  adj[2][3].first = 5;
  adj[2][3].second = 4;
  
  adj[1][3].first = 4;
  adj[1][3].second = 3;
  */
  /*
  adj[0][1].first = 3;
  adj[0][1].second = 2;

  adj[0][2].first = 5;
  adj[0][2].second = 2;
  
  adj[2][3].first = 6;
  adj[2][3].second = 2;
  
  adj[1][3].first = 4;
  adj[1][3].second = 2;
  */
  adj[0][1].first = 10;
  adj[0][1].second = 0;
  
  adj[0][2].first = 3;
  adj[0][2].second = 1;
  
  adj[1][3].first = 8;
  adj[1][3].second = 2;
  
  adj[2][4].first = 7;
  adj[2][4].second = 5;
  
  adj[3][2].first = 4;
  adj[3][2].second = 2;
  
  adj[3][5].first = 6;
  adj[3][5].second = 2;
  
  adj[4][1].first = 3;
  adj[4][1].second = 1;
  
  adj[4][5].first = 5;
  adj[4][5].second = 3;
  MaxFlowWithBound(adj, 0, size - 1);
  return 0;
}
