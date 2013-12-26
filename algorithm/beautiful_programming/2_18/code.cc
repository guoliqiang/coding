// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-24 21:09:16
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"
/*
1. 有一个无序、元素个数为2n的正整数数组，要求：如何能把这个数组分割为两个子数组，子数组的元素个数不限，并使两个子数组之和最接近。
   数组之和为sum，此问题转换为背包为sum/2，找出起能装的最重的一个组合.
2. 有一个无序、元素个数为2n的正整数数组，要求：如何能把这个数组分割为元素个数为n的两个两个子数组之和最接进。
   三维背包问题：dp[i][j][k]表示前i个里面选择j个，得到的值不超过k且最接近k

note: 如果输入数组中有负数，先全部转换成正数。

变形问题:
两个数组a和b都有n个元素，规定每次操作只能交换a和b的某一个元素，即两个数组永远都是n个元素。
如何最快的达到 | sum(a) - sum( b ) | 最小？

需要在结果中找出一个交换次数最少的

*/
namespace algorithm {
void Trace(std::vector<std::vector<int> > & dp, std::vector<int> & v, std::vector<int> & rs, int x, int y) {
  if (x == 0 || y == 0) return;
  if (dp[x][y] == dp[x - 1][y - v[x]] + v[x]) {
    rs.push_back(v[x]);
    Trace(dp, v, rs, x - 1, y - v[x]);
  } else {
    Trace(dp, v, rs, x - 1, y);
  }
}

// 0, 1背包问题
std::vector<int> Dp(std::vector<int> v) {
  v.insert(v.begin(), 0);
  int n = v.size();
  int sum = 0;
  for (int i = 1; i < n; i++) {
    sum += v[i];
  }
  sum /= 2;
  std::vector<std::vector<int> > dp(sum + 1, std::vector<int>(n, 0));
  for (int i = 1; i < n; i++) {
    for (int j = sum; j >= v[i]; j--) {
      dp[i][j] = std::max(dp[i - 1][j], dp[i - 1][j - v[i]] + v[i]);
    }
  }
  std::vector<int> rs;
  Trace(dp, v, rs, n - 1, sum);
  return rs;
}

void Trace(std::vector<int> & v, std::vector<std::vector<std::vector<int> > > & dp, std::vector<int> & rs, int x, int y, int k) {
  if (x == 0 || y == 0 || k == 0) return;
  if (dp[x][y][k] == dp[x - 1][y - 1][k - v[x]] + v[x]) {
    rs.push_back(v[x]);
    Trace(v, dp, rs, x - 1, y - 1, k - v[x]);
  } else {
    Trace(v, dp, rs, x - 1, y, k);
  }
}

// v.size() 必须是偶数
std::vector<int> Dp2(std::vector<int> v) {
  v.insert(v.begin(), 0);
  int n = v.size();
  int sum = 0;
  for (int i = 1; i < n; i++) sum += v[i];
  sum /= 2;
  std::vector<std::vector<std::vector<int> > > dp(n, std::vector<std::vector<int> >(n / 2 + 1, std::vector<int>(sum + 1, 0)));
  for (int i = 1; i < n; i++) {
    for (int j = 1; j <= std::min(i, n / 2); j++) {
      for (int k = sum; k >= v[i]; k--) {
        dp[i][j][k] = std::max(dp[i - 1][j][k], dp[i - 1][j - 1][k - v[i]] + v[i]);
      }
    }
  }
  std::vector<int> rs;
  Trace(v, dp, rs, n - 1, n / 2, sum);
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  std::vector<int> v;
  v.push_back(5);
  v.push_back(1);
  v.push_back(2);
  v.push_back(2);
  std::vector<int> rs = Dp(v);
  LOG(INFO) << JoinVector(rs);
  rs = Dp2(v);
  LOG(INFO) << JoinVector(rs);
  return 0;
}
