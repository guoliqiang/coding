// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-17 17:12:25
// File  : code.cc
// Brief :

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
    LOG(INFO) << "one flow:";
    for (int i = target; i != source; i = pre[i]) {
      std::cout << pre[i] << "-" << i << " ";
      min = std::min(min, flow[pre[i]][i]);
    }
    std::cout << " and flow = " << min << std::endl;
    for (int i = target; i != source; i = pre[i]) {
      flow[pre[i]][i] -= min;
      flow[i][pre[i]] += min;
    }
    // LOG(INFO) << "cur flow:" << min;
    rs += min;
  }
  return rs;
}

void MaxFlowWithBound(std::vector<std::vector<std::pair<int, int> > > & adj, int source, int target) {
  N = adj.size() + 2;
  memset(matrix, 0, sizeof(up));
  memset(matrix, 0, sizeof(low));
  memset(matrix, 0, sizeof(sum_in));
  memset(matrix, 0, sizeof(sum_out));
  for (int i = 0; i < adj.size(); i++) {
    for (int j = 0; j < adj.size(); j++) {
      up[i][j] = adj[i][j].first;
      low[i][j] = adj[i][j].second;
      sum_in[j] += low[i][j];
      sum_out[i] += low[i][j];
      matrix[i][j] = up[i][j] - low[i][j];
    }
  }
  low[target][source] = 0;
  up[target][source] = INT_MAX;
  matrix[target][source] = INT_MAX;
  // ss = N - 2  tt == N - 1
  int ss_out = 0;
  for (int i = 0; i < adj.size(); i++) {
    int t = sum_in[i] - sum_out[i];
    // LOG(INFO) << i << ":" << t;
    if (t > 0) matrix[N - 2][i] = t;
    if (t < 0) {
      matrix[i][N - 1] = -t;
      ss_out += -t;
    }
  }
  OutMatrix();
  LOG(INFO) << "first make max flow";
  int max_flow = MaxFlow(N - 2, N - 1);
  LOG(INFO) << "first max flow: " << max_flow << " ss out:" << ss_out;
  if (max_flow != ss_out)  {
    LOG(INFO) << "infesiable";
    return;
  }
  // OutFlow();
  up[target][source] = 0;
  N -= 2;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0 && flow[i][j] < matrix[i][j]) {
        LOG(INFO) << "flow[" << i <<"][" << j << "]:" << flow[i][j];
        LOG(INFO) << "matrix[" << i <<"][" << j << "]:" << matrix[i][j];
        flow[i][j] = low[i][j] + (matrix[i][j] - flow[i][j]);
        LOG(INFO) << "low[" << i <<"][" << j << "]:" << low[i][j];
        LOG(INFO) << "flow[" << i <<"][" << j << "]:" << flow[i][j];
      } else if (matrix[i][j] > 0) {
        flow[i][j] = low[i][j];
      } else {
        flow[i][j] = 0;
      }
    }
  }
  max_flow = 0;
  for (int i = 0; i < N; i++) {
    max_flow += flow[source][i];
  }
  LOG(INFO) << " flow matrix:";
  LOG(INFO) << "out flow:";
  OutFlow(N);
  HERE(INFO);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (up[i][j] > 0) {
        matrix[i][j] = up[i][j] - flow[i][j];
        matrix[j][i] = flow[i][j] - low[i][j];
      }
    }
  }
  LOG(INFO) << "second max flow";
  OutMatrix();
  max_flow += MaxFlow(source, target);
  HERE(INFO);
  OutFlow();

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
