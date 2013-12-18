// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-17 14:30:49
// File  : code.cc
// Brief :

/*
Accepted  P1214.cppFrom guoliqiang  
100
0 ms
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 105;
const int MAXW = 10005;
int N;
int W;

struct Node {
 int max;
 int min;
} dp[MAXW][2], dp2[MAXW];

int data[MAXN];

std::pair<int, int> Dp() {
  memset(dp, 0, sizeof(dp));
  bool flag = false;
  for (int i = 1; i <= N; i++) {
    for (int j = 0; j <= W; j++) {
      dp[j][!flag] = dp[j][flag];
      
      if (j >= data[i]) {
        if (j == data[i]) {
          dp[j][!flag].max = dp[j][!flag].max == 0 ? 1 : std::max(1, dp[j][!flag].max);
          dp[j][!flag].min = dp[j][!flag].min == 0 ? 1 : std::min(1, dp[j][!flag].min);
        } else {
          int tmax = dp[j - data[i]][!flag].max;
          int tmin = dp[j - data[i]][!flag].min;
          if (tmax > 0) {
            dp[j][!flag].max = dp[j][!flag].max == 0 ? tmax + 1 : std::max(tmax + 1, dp[j][!flag].max);
          }
          if (tmin > 0) {
            dp[j][!flag].min = dp[j][!flag].min == 0 ? tmin + 1 : std::min(tmin + 1, dp[j][!flag].min);
          }
        }
      }
    }
    flag = !flag;
  }
  return std::make_pair(dp[W][flag].min, dp[W][flag].max);
}

// AC
std::pair<int, int> DpS() {
  memset(dp, 0, sizeof(dp));
  for (int i = 0; i < 2; i++) {
    for (int j = 1; j <= W; j++) {
      dp[j][i].max = -INF;
      dp[j][i].min = INF;
    }
  }

  bool flag = false;
  for (int i = 1; i <= N; i++) {
    for (int j = 0; j <= W; j++) {
      dp[j][!flag] = dp[j][flag];
      if (j >= data[i]) {
        dp[j][!flag].max = std::max(dp[j][!flag].max, dp[j - data[i]][!flag].max + 1);
        dp[j][!flag].min = std::min(dp[j][!flag].min, dp[j - data[i]][!flag].min + 1);
      }
    }
    flag = !flag;
  }
  return std::make_pair(dp[W][flag].min, dp[W][flag].max);
}

std::pair<int, int> DpNB() {
  memset(dp2, 0, sizeof(dp2));
  for (int i = 1; i <= W; i++) {
    dp2[i].max = -INF;
    dp2[i].min = INF;
  }
  for (int j = 1; j <= N; j++) {
    for (int i = data[j]; i <= W; i++) {
      dp2[i].max = std::max(dp2[i].max, dp2[i - data[j]].max + 1);
      dp2[i].min = std::min(dp2[i].min, dp2[i - data[j]].min + 1);
    }
  }
  return std::make_pair(dp2[W].min, dp2[W].max);
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    scanf("%d", &W);
    getchar();
    for (int i = 0; i < N; i++) {
      int t = 0;
      scanf("%d", &t);
      data[i + 1] = t;
    }
    std::pair<int, int> t = DpNB();
    printf("%d\n%d\n", t.first, t.second);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
