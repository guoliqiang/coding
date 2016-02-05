// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-26 10:22:25
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"
// 图上的点代表城市，从一个城市到另一个要花一周(瞬间移动，e.g.第3周在A,第4周在B,前提是AB有边)。
// 每个城市每周会有一些假日，如果某周在某城市停留，会获得相应数量的假日。起始在某一城市，问第n周在k城市时，能获得的最多假日数量.
//
// 写法很 NB, 可以作为BFS做DP的模板
namespace algorithm {
int Get(std::vector<std::vector<int> > & matrix, int start, int week, int end, std::vector<int> & v) {
  int n = v.size();
  std::vector<std::vector<int> > dp(week + 1, std::vector<int>(n, -1));
  dp[0][start] = 0;
  for (int i = 1; i <= week; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        if (matrix[k][j] == 1 && dp[i - 1][k] != -1) {
          dp[i][j] = std::max(dp[i][j], dp[i - 1][k] + v[j]);
        }
      }
    }
  }
  return dp[week][end];
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
