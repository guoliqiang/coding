// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-03 23:15:09
// File  : code.cc
// Brief :

/* ST(sparse table) 算法
 * 比之前常写的rmq效率更高，n * log(n) 的预处理复杂度，
 * 但回答查询只需要O(1)的时间复杂度
 * dp[i][j] 表示从data[i]开始连续2^j个数中的最值
 * dp[i][j] = max(dp[i][j - 1], dp[m][j - 1])  m = i + 2^(j - 1)
 *
 *
 *
 *
 * 直观的矩阵方法(f[i][j]保存 i～j之间的最值，其处理时间复杂度为O(n^2))
 * 回答查询的时间复杂度为O(1)
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int data[MAX] = {0};
int dp[MAX][MAX] = {{0}};

void Build() {
 for (int i = 0; i < N; i++) dp[i][0] = data[i];
 for (int j = 1; (1 << j) < N; j++) {
   int t = 1 << j;
   for (int i = 0; i < N && i + t - 1 < N; i++) {
     int k = 1 << (j - 1);
     dp[i][j] = std::max(dp[i][j - 1], dp[i + k][j - 1]);
   }
 }
}

int Get(int x, int y) {
  double t = y - x + 1;
  int k = log(t)/log(2.0);
  return std::max(dp[x][k], dp[y - (1 << k) + 1][k]);
}

void Read(std::vector<int> & v) {
  N = v.size();
  memset(data, 0, sizeof(data));
  memset(dp, 0, sizeof(dp));
  for (int i = 0; i < N; i++) {
    data[i] = v[i];
  }
  Build();
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(3);
  v.push_back(5);
  v.push_back(-2);
  v.push_back(4);
  v.push_back(10);
  v.push_back(8);
  v.push_back(23);
  v.push_back(0);
  v.push_back(-10);
  Read(v);
  LOG(INFO) << Get(0, 9);
  return 0;
}
