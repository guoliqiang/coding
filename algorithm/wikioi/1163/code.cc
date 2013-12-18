// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-18 11:14:24
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 102;
const int MAXS = 1250;
int S;
int left[MAXN];
int right[MAXN];
int data[MAXN];
int time[MAXN];
int num = 0;
int dp[MAXN][MAXS];

int Build() {
  num++;
  int rs = num;
  int t1, t2;
  scanf("%d%d", &t1, &t2);
  time[rs] = t1 * 2;
  data[rs] = t2;
  if (t2 == 0) {
    left[rs] = Build();
    right[rs] = Build();
  }
  return rs;
}

void BuildWrapper() {
  num = 0;
  memset(left, 0, sizeof(left));
  memset(right, 0, sizeof(right));
  memset(data, 0, sizeof(data));
  memset(time, 0, sizeof(time));
  memset(dp, -1, sizeof(dp));
  Build();
  getchar();
}

int Dp(int r, int t) {
  if (t - time[r] <= 0) dp[r][t] = 0;
  if (dp[r][t] >= 0) return dp[r][t];
  
  if (left[r] == 0 && right[r] == 0) {
    int foo = (t - time[r]) / 5;
    dp[r][t] = std::min(foo, data[r]);
  } else {
    int max = 0;
    for (int i = 0; i <= t - time[r]; i++) {
      int tmax = 0;
      if (left[r] != 0) tmax += Dp(left[r], i);
      if (right[r] != 0) tmax += Dp(right[r], t - time[r] - i);
      max = std::max(max, tmax);
    }
    dp[r][t] = max;
  }
  return dp[r][t];
}

void Read() {
  while (scanf("%d", &S) != EOF) {
    getchar();
    BuildWrapper();
    int t = Dp(1, S);
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
