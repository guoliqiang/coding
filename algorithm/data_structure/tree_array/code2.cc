// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-03 19:09:48
// File  : code2.cc
// Brief :

/*
 * 之前的做法 dp
 * dp[i][j]表示data[0][0]到data[i][j] 子矩阵的和
 * dp[i][j] = dp[i][j - 1] + dp[i - 1][j] - dp[i - 1][j - 1] + data[i][j]
 *
 * 缺点：更新操作代价大
 *
 * */
#include "base/public/common_head.h"

// 二维树状数组
namespace algorithm {
const int MAX = 1000;
int M = 0;
int N = 0;
int data[MAX][MAX] = {{0}};
int c[MAX][MAX] = {{0}};

int Low(int k) {
  int t = k & (k ^ (k - 1));
  return t;
}

/*
 * 返回data[1][1] ~ data[x][y]的和
 * */
int GetSum(int x, int y) {
 int rs = 0;
 for (int i = x; x > 0; x -= Low(x)) {
    for (int j = y; j > 0; j -= Low(j)) {
      rs += c[i][j];
    }
  }
  return rs;
}

void Add(int x, int y, int v) {
  data[x][y] += v;
  for (int i = x; x <= M; x += Low(x)) {
    for (int j = y; j <= N; j += Low(j)) {
      c[i][j] += v;
    }
  }
}

void Read(std::vector<std::vector<int> > & v) {
  M = v.size();
  N = (M == 0) ? 0 : v[0].size();
  memset(data, 0, sizeof(data));
  memset(c, 0, sizeof(c));
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      data[i + 1][j + 1] =  v[i][j];
    }
  }
  for (int i = 1; i <= M; i++) {
    for (int j = 1; j <= N; j++) {
      int t = 0;
      for (int k = i - Low(i) + 1; k <= i; k++) {
        t += data[k][j];
      }
      c[i][j] = t;
    }
    for (int j = 1; j <= N; j++) {
      int t = 0;
      for (int k = j - Low(j) + 1; k <= j; k++) {
        t += c[i][k];
      }
      c[i][j] = t;
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int m = 3;
  int n = 3;
  std::vector<std::vector<int> > v(m, std::vector<int>(n, 0));
  v[0][0] = 1; v[0][1] = 2; v[0][2] = 1;
  v[1][0] = 4; v[1][1] = 9; v[1][2] = 3;
  v[2][0] = 6; v[2][1] = 3; v[2][2] = 4;
  Read(v);
  LOG(INFO) << GetSum(2, 2);
  Add(2, 1, -10);
  LOG(INFO) << GetSum(2, 2);
  return 0;
}
