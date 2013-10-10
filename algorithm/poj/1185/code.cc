// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-10 14:51:36
// File  : code.cc
// Brief :

/* Accepted!
 * 状态压缩dp
 *
 * 炮兵阵地
 * http://poj.org/problem?id=1185
 * 司令部的将军们打算在N*M的网格地图上部署他们的炮兵部队。一个N*M的地图由N行M列组成，
 * 地图的每一格可能是山地（用"H" 表示），也可能是平原（用"P"表示），如下图。在每一格
 * 平原地形上最多可以布置一支炮兵部队（山地上不能够部署炮兵部队）；一支炮兵部队在地
 * 图上的攻击范围如图中黑色区域所示： 
 *
 *
 * 如果在地图中的灰色所标识的平原上部署一支炮兵部队，则图中的黑色的网格表示它能够攻
 * 击到的区域：沿横向左右各两格，沿纵向上下各两格。图上其它白色网格均攻击不到。从图
 * 上可见炮兵的攻击范围不受地形的影响。 现在，将军们规划如何部署炮兵部队，在防止误伤
 * 的前提下（保证任何两支炮兵部队之间不能互相攻击，即任何一支炮兵部队都不在其他支炮
 * 兵部队的攻击范围内），在整个地图区域内最多能够摆放多少我军的炮兵部队。 
 * */

#include "base/public/common_head.h"

namespace algorithm {

bool IsValid(int n) {
  if (n & n << 1) return false;
  if (n & n << 2) return false;
  return true;
}

int OneNum(int n) {
  int rs = 0;
  while (n) {
    rs++;
    n &= n - 1;
  }
  return rs;
}

void FindValidSet(int n, std::vector<std::pair<int, int> > & valid) {
  for (int i = 0; i < (1 << n); i++) {
    if (IsValid(i)) {
      valid.push_back(std::make_pair(i, OneNum(i)));
    }
  }
}

void ConvertMatrix(std::vector<std::string> & matrix,
                   std::vector<int> &v) {
  for (int i = 0; i < matrix.size(); i++) {
    int t = 0;
    for (int j = 0; j < matrix[i].size(); j++) {
      if (matrix[i][j] == 'H') t |= 1 << (matrix[i].size() - j - 1);
    }
    v.push_back(t);
  }
}

int DP(std::vector<std::string> & matrix) {
  std::vector<int> v;
  ConvertMatrix(matrix, v);
  std::vector<std::pair<int, int> > valid;
  FindValidSet(matrix[0].size(), valid);
  int m = matrix.size();
  int n = valid.size();

  std::vector<std::vector<std::vector<int> > > dp(m,
              std::vector<std::vector<int> >(n, std::vector<int>(n, 0)));
  for (int i = 0; i < n; i ++) {
    for (int j = 0; j < n; j++) {
      if (valid[i].first & v[0]) continue;
      dp[0][i][j] = valid[i].second;
    }
  }

  for (int i = 1; i < m; i++) {  // 第几行
    for (int j = 0; j < n; j++) {
      if (valid[j].first & v[i]) continue;
      for (int k = 0; k < n; k++) {
        if (valid[k].first & v[i - 1]) continue;
        if (valid[j].first & valid[k].first) continue;
        for (int l = 0; l < n; l++) {
          if (i - 2 >= 0 && valid[l].first & v[i - 2]) continue;
          if (i - 2 >= 0 && valid[j].first & valid[l].first) continue;
          if (dp[i][j][k] < dp[i - 1][k][l] + valid[j].second)
            dp[i][j][k] = dp[i - 1][k][l] + valid[j].second;
        }
      }
    }
  }
  int max = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      max = std::max(max, dp[m - 1][i][j]);
    }
  }
  return max;
}

int ReadFromConsole() {
  int m = 0;
  int n = 0;
  std::vector<std::string> matrix;
  std::cin >> m >> n;
  for (int i = 0; i < m; i++) {
    std::string t;
    std::cin >> t;
    matrix.push_back(t);
  }
  return DP(matrix);
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::string> matrix;
  matrix.push_back("PHPP");
  matrix.push_back("PPHH");
  matrix.push_back("PPPP");
  matrix.push_back("PHPP");
  matrix.push_back("PHHP");
  LOG(INFO) << DP(matrix);
  LOG(INFO) << ReadFromConsole();
  return 0;
}
