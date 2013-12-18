// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-16 21:57:06
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1005;
int N = 0;
struct Node {
  int w;
  int l;
} data[MAXN];

int dp[MAXN];

bool Cmp(const Node & x, const Node & y) {
  return x.w < y.w || (x.w == y.w && x.l < y.l);
}

// 不能使用O(n*log(n))的方法求递增子序列，test.txt就是个反例
int Dp() {
  std::sort(data, data + N, Cmp);
  memset(dp, 0, sizeof(dp));
  int rs = 0;
  for (int i = 0; i < N; i++) {
    int max = 0;
    for (int j = 0; j < i; j++) {
      if (data[j].w < data[i].w && data[j].l < data[i].l) max = std::max(max, dp[j]);
    }
    dp[i] = max + 1;
    rs = std::max(rs, dp[i]);
  }
  return rs;
}


void Read() {
  int T = 0;
  scanf("%d", &T);
  getchar();
  for (int k = 0; k < T; k++) {
    scanf("%d", &N);
    getchar();
    for (int i = 0; i < N; i++) {
      int w, l;
      scanf("%d%d", &w, &l);
      getchar();
      data[i].w = std::min(w, l);
      data[i].l = std::max(w, l);
    }
    int t = Dp();
    printf("%d\n", t);
  }
}
}  // namespace algorithm

namespace NB {
const int MAXN = 1005;
int N = 0;
struct Node {
  int w;
  int l;
} data[MAXN];

int matrix[MAXN][MAXN];
int dp[MAXN];

int Deep(int k) {
  if (dp[k] != 0) return dp[k];
  dp[k] = 1;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i]) dp[k] = std::max(dp[k], Deep(i) + 1);
  }
  return dp[k];
}

int Deep() {
  memset(dp, 0, sizeof(dp));
  int rs = 0;
  for (int i = 0; i < N; i++) rs = std::max(rs, Deep(i));
  return rs;
}

void Read() {
  int T = 0;
  scanf("%d", &T);
  getchar();
  for (int k = 0; k < T; k++) {
    scanf("%d", &N);
    getchar();
    for (int i = 0; i < N; i++) {
      int w, l;
      scanf("%d%d", &w, &l);
      getchar();
      data[i].w = std::min(w, l);
      data[i].l = std::max(w, l);
    }
    memset(matrix, 0, sizeof(matrix));
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (data[i].w < data[j].w && data[i].l < data[j].l) matrix[i][j] = 1;
      }
    }
    int t = Deep();
    printf("%d\n", t);
  }
}

}  // namespace NB
using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  // Read();
  NB::Read();
  return 0;
}
