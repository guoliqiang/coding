// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-27 00:24:53
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// Accepted 136K  0MS
// 变相求最大团的size
namespace algorithm {
const int MAXN = 205;
int N;
struct Node {
  int b;
  int e;
} data[MAXN];

int dp[MAXN * 2];

int Dp() {
  memset(dp, 0, sizeof(dp));
  int maxe = 0;
  int minb = INF;
  for (int i = 0; i < N; i++) {
    int b = data[i].b;
    int e = data[i].e;
    if (b % 2 == 0) b--;
    if (e % 2 == 1) e++;
    maxe = std::max(e, maxe);
    minb = std::min(b, minb);
    for (int j = b; j <= e; j++) dp[j]++;
  }
  int rs = 0;
  for (int i = minb; i <= maxe; i++) rs = std::max(rs, dp[i]);
  return rs;
}

void Read() {
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int k = 0; k < c; k++) {
    memset(data, 0, sizeof(data));
    scanf("%d", &N);
    getchar();
    for (int i = 0; i < N; i++) {
      scanf("%d%d", &(data[i].b), &(data[i].e));
      if (data[i].b > data[i].e) std::swap(data[i].b, data[i].e);
      getchar();
    }
    int t = Dp();
    printf("%d\n", t * 10);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
