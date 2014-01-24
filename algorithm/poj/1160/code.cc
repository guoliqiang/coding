// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-24 20:32:36
// File  : code.cc
// Brief :

// Accepted 568K  63MS
//
// 一个重要结论：一条直线上存在若干个点，找出一个点，其余各个点到此点的距离之和最短
// 如果中间的点只有一个（点的总数为奇数），则为此点，如果中间点是两个（点总数为偶数），则可以选择两者中的任意一个

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 305;
int N;
const int MAXM = 32;
int M;

int data[MAXN];
int dp[MAXM][MAXN];
int dis[MAXN][MAXN];

int Dp() {
  std::sort(data, data + N);
  memset(dis, 0, sizeof(dis));
  for (int i = 0; i < N; i++) {
    for (int j = i; j < N; j++) {
      int mid = i + (j - i) / 2;
      int t = 0;
      for (int l = i; l <= j; l++) t += abs(data[l] - data[mid]);
      dis[i][j] = t;
    }
  }
  memset(dp, 0, sizeof(dp));
  for (int i = 0; i < M; i++) {
    for (int j = i; j < N; j++) {
      if (i == 0) dp[i][j] = dis[i][j];
      else {
        dp[i][j] = INF;
        for (int k = i - 1; k < j; k++) {
          dp[i][j] = std::min(dp[i][j], dp[i - 1][k] + dis[k + 1][j]);
        }
      }
    }
  }
  return dp[M - 1][N - 1];
}

int DpTLE() {
  std::sort(data, data + N);
  memset(dp, 0, sizeof(dp));
  for (int i = 0; i < M; i++) {
    for (int j = i; j < N; j++) {
      dp[i][j] = INF;
      for (int k = j; k >= i; k--) {
        int t = 0;
        for (int l = k; l <= j; l++) t += data[l] - data[k];
        
        if (i == 0) {
          for (int l = k; l >= 0; l--) t += data[k] - data[l];
          if (t < dp[i][j]) dp[i][j] = t;
        } else {
          for (int l = k; l > i - 1; l--) {
            t += data[k] - data[l];
            if (t + dp[i - 1][l - 1] < dp[i][j]) dp[i][j] = t + dp[i - 1][l - 1];
          }
        }
      }
      // LOG(INFO) << "dp[" << i << "][" << j << "]:" << dp[i][j];
    }
  }
  return dp[M - 1][N - 1];
}

void Read() {
  scanf("%d%d", &N, &M);
  for (int i = 0; i < N; i++) scanf("%d", &(data[i]));
  int t = Dp();
  printf("%d\n", t);
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
