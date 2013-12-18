// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-17 14:06:18
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1007;
int N;
struct Node {
  int b;
  int e;
  int v;
} data[MAXN];
int dp[MAXN];

bool Cmp(const Node & x, const Node & y) {
  return x.b < y.b;
}

int Dp(){
  std::sort(data, data + N, Cmp);
  memset(dp, 0, sizeof(dp));
  int max = 0;
  for (int i = 0; i < N; i++) {
    int t = 0;
    for (int j = 0; j < i; j++) {
      if (data[j].e <= data[i].b) t = std::max(t, dp[j]);
    }
    dp[i] = t + data[i].v;
    max = std::max(max, dp[i]);
  }
  return max;
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    for (int i = 0; i < N; i++) {
      int b, e, v;
      scanf("%d%d%d", &b, &e, &v);
      getchar();
      data[i].b = b;
      data[i].e = e;
      data[i].v = v;
    }
    int t = Dp();
    printf("%d\n", t);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
