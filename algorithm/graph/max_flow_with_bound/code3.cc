// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-14 22:46:03
// File  : code.cc
// Brief :

// 有源汇的带上下界的最大流

/*
 * 2. 有源汇有上下界的最大流
 *    源点st，终点sd。超级源点ss，超级终点dd。
 *    首先判断是否存在满足所有边上下界的可行流，方法可以转化成无源汇有上下界的可行流问题。如下：
 *    
 *    增设一条从d到s没有下界容量为无穷的边，那么原图就变成了一个无源汇的循环流图。接下来的事情一样，
 *    超级源点ss连i（du[i]>0），i连超级汇点（du[i]<0）,对（ss，dd）进行一次最大流，
 *    当maxflow等于所有(du[]>0)之和时，有可行流，否则没有。
 *
 *    当有可行流时，删除超级源点ss和超级终点dd，再对（s，d）进行一次最大流，此时得到的maxflow则为题目的解。
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 100;
int st = 0;
int sd = 2;
int sst = 3;
int ssd = 4;

int flow[MAXN][MAXN];
int du[MAXN];
int down[MAXN][MAXN];
int visited[MAXN];
int pre[MAXN];

int MaxFlow(int source, int target) {
  int rs = 0;
  while (true) {
    memset(visited, 0, sizeof(visited));
    memset(pre, 0, sizeof(pre));
    std::queue<int> queue;
    queue.push(source);
    visited[source] = 1;
    while (!queue.empty()) {
      int cur = queue.front();
      queue.pop();
      if (cur == target) break;
      for (int i = 0; i < MAXN; i++) {
        if (flow[cur][i] > 0 && visited[i] == 0) {
          visited[i] = 1;
          queue.push(i);
          pre[i] = cur;
        }
      }
    }
    if (visited[target] == 0) break;
    int min = INF;
    for (int i = target; i != source; i = pre[i]) {
      min = std::min(min, flow[pre[i]][i]);
    }
    for (int i = target; i != source; i = pre[i]) {
      flow[pre[i]][i] -= min;
      flow[i][pre[i]] += min;
    }
    rs += min;
  }
  return rs;
}

void OutFlow(int n) {
  LOG(INFO) << "Flow:";
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cout << flow[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void Read(std::vector<std::vector<std::pair<int, int> > > & matrix) {
  int n = matrix.size();
  memset(flow, 0, sizeof(flow));
  memset(du, 0, sizeof(du));
  memset(down, 0, sizeof(down));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      flow[i][j] = matrix[i][j].second - matrix[i][j].first;
      down[i][j] = matrix[i][j].first;
      du[i] -= matrix[i][j].first;
      du[j] += matrix[i][j].first;
    }
  }
  OutFlow(n + 2);
  int full = 0;
  for (int i = 0; i < n; i++) {
    if (du[i] > 0) {
      flow[sst][i] = du[i];
      full += du[i];
    } else if (du[i] < 0) {
      flow[i][ssd] = -du[i];
    }
  }
  flow[sd][st] = INF;  
  OutFlow(n + 2);
  int t = MaxFlow(sst, ssd);
  LOG(INFO) << "first:" << t;
  OutFlow(n + 2);
  flow[sd][st] = 0;
  OutFlow(n + 2);
  t = MaxFlow(st, sd);
  // 这里的t就是最大流，不用加上first的结果，
  // 因为通过反悔边(st -> sd),first上的结果被计算了进去
  OutFlow(n + 2);
  LOG(INFO) << "second:" << t;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (matrix[i][j].second > 0) {  // 每条路径上的流值，注意用的是flow[j][i]
        LOG(INFO) << i << "~" << j << ":" << down[i][j] + flow[j][i];
      }
    }
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  int n = 3;
  std::vector<std::vector<std::pair<int, int> > >  matrix(n, std::vector<std::pair<int, int> >(n, std::make_pair(0, 0)));
  matrix[0][1].first = 3;
  matrix[0][1].second = 8;
  
  matrix[1][2].first = 4;
  matrix[1][2].second = 8;

  Read(matrix);
  return 0;
}
