// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-13 19:48:55
// File  : code.cc
// Brief :
// http://www.cnblogs.com/kakamilan/archive/2012/07/10/2583903.html

/*
桥  == 割边
删除一条边后就能令此无向图变的不联通。

有的图只有删除多条边才能令此无向图变的不联通，这些边成为割边集，一个图可能有
若干个割边集，一般要求找出最小的割边集合。

割边集合与桥的对应关系：
如果某个割边集的大小为1，其就是桥(割边)


注意：只有无向图才有桥(割边)

*/

#include "base/public/common_head.h"
#include "../base/graph.h"

namespace algorithm {

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

std::vector<std::pair<int, int> > FindBridge(std::vector<std::vector<int> > & matrix,
                                             int begin = 0) {
  int dfs_clock = 0;
  std::vector<std::pair<int, int> > re;
  std::vector<int> lowu(matrix.size(), -1);
  std::vector<int> dfsn(matrix.size(), -1);
  FindBridgeUnDirected(matrix, lowu, dfsn, 0, dfs_clock, -1, re);
  return re;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix;
  AdjaMatrix(matrix, 6);
  SetMatrixDouble(matrix, 0, 1, 1);
  SetMatrixDouble(matrix, 1, 2, 1);
  SetMatrixDouble(matrix, 2, 3, 1);
  SetMatrixDouble(matrix, 3, 1, 1);
  SetMatrixDouble(matrix, 0, 4, 1);
  SetMatrixDouble(matrix, 4, 5, 1);
  SetMatrixDouble(matrix, 5, 1, 1);
  std::vector<std::pair<int, int> > rs;
  rs = FindBridge(matrix, 0);
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i].first << " " << rs[i].second;
  }
  return 0;
}
