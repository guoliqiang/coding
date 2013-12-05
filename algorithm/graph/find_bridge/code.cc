// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-13 19:48:55
// File  : code.cc
// Brief :
// http://www.cnblogs.com/kakamilan/archive/2012/07/10/2583903.html

#include "base/public/common_head.h"
#include "../base/graph.h"

namespace algorithm {

void FindBridgeDirected(std::vector<std::vector<int> > &matrix,
                        std::vector<int> & lowu,
                        std::vector<int> & dfsn,
                        int cur,
                        int & dfs_clock,
                        std::vector<std::pair<int, int> > & bridge) {
  lowu[cur] = dfsn[cur] = ++dfs_clock;
  for (int i = 0; i < matrix.size(); i++) {
    if (matrix[cur][i] == 0) continue;
    if (dfsn[i] == -1) {
      FindBridgeDirected(matrix, lowu, dfsn, i, dfs_clock, bridge);
      lowu[cur] = std::min(lowu[cur], lowu[i]);
      if (lowu[i] > dfsn[cur]) {  // lowu[i] > lowu[cur]　这种写法是错误的
        bridge.push_back(std::make_pair(cur, i));
      }
    } else {
      // 两种写法都可以，第二种写法更有实际意义，其意义和tarjan一样
      // 表示能到达的最低节点
      // lowu[cur] = std::min(lowu[cur], dfsn[i]);
      lowu[cur] = std::min(lowu[cur], lowu[i]);
    }
  }
}

void FindBridgeUnDirected(std::vector<std::vector<int> > &matrix,
                          std::vector<int> & lowu,
                          std::vector<int> & dfsn,
                          int cur, int & dfs_clock, int father,
                          std::vector<std::pair<int, int> > & bridge) {
  lowu[cur] = dfsn[cur] = ++dfs_clock;
  for (int i = 0; i < matrix.size(); i++) {
    if (matrix[cur][i] == 0 || i == father) continue;
    if (dfsn[i] == -1) {
      FindBridgeUnDirected(matrix, lowu, dfsn, i, dfs_clock, cur, bridge);
      lowu[cur] = std::min(lowu[cur], lowu[i]);
      if (lowu[i] > dfsn[cur]) {
        bridge.push_back(std::make_pair(cur, i));
      }
    } else {
      lowu[cur] = std::min(lowu[cur], dfsn[i]);
    }
  }
}

std::vector<std::pair<int, int> > FindBridge(std::vector<std::vector<int> > & matrix,
                                             int begin = 0,
                                             bool directed = true) {
  int dfs_clock = 0;
  std::vector<std::pair<int, int> > re;
  std::vector<int> lowu(matrix.size(), -1);
  std::vector<int> dfsn(matrix.size(), -1);
  if (directed) {
    FindBridgeDirected(matrix, lowu, dfsn, 0, dfs_clock, re);
  } else {
    FindBridgeUnDirected(matrix, lowu, dfsn, 0, dfs_clock, -1, re);
  }
  return re;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix;
  AdjaMatrix(matrix, 5);
  SetMatrix(matrix, 0, 1, 2);
  SetMatrix(matrix, 2, 0, 5);
  SetMatrix(matrix, 0, 3, 1);
  SetMatrix(matrix, 1, 4, 3);
  SetMatrix(matrix, 4, 2, 4);
  std::vector<std::pair<int, int> > rs;
  rs = FindBridge(matrix);
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i].first << " " << rs[i].second;
  }
  return 0;
}
