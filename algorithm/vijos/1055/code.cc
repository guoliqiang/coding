// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-18 12:55:02
// File  : code.cc
// Brief :

// http://www.cnblogs.com/ka200812/archive/2012/09/30/2709188.html
// can AC

// TLE
// TODO 离散化处理
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 30005;
int dp[MAXN];
int L;
int W;
int N;
int stack[MAXN];
int before[MAXN];
std::map<int, std::set<int> > v;

int MaxV() {
  memset(stack, 0, sizeof(stack));
  int top = 0;
  int i = 0;
  int max = 0;
  while (i <= W + 1) {
    if (top == 0 || dp[i] >= dp[stack[top - 1]]) {
      stack[top++] = i;
      i++;
    } else {
      int t = dp[stack[top - 1]];
      top--;
      int k = i;
      if (i == W + 1) k -= 1;
      int tmax = top == 0 ? t * k : t * (k - stack[top - 1]);
      max = std::max(max, tmax);
      // LOG(INFO) << "max:" << max << " at:" << i;
    }
  }
  return max;
}

int Dp() {
  int max = 0;
  memset(dp, 0, sizeof(dp));
  memset(before, 0, sizeof(before));
  for (int i = 1; i <= L; i++) {
    if (i != L && v.count(i) == 0) continue;
    for (int j = 0; j <= W; j++) {
      dp[j] = i - before[j];
      if (v.count(i) && v[i].count(j)) before[j] = i;
    }
    int tmax = MaxV();
    max = std::max(max, tmax);
    /*
    LOG(INFO) << "i:" << i;
    for (int j = 0; j <= W; j++) {
      LOG(INFO) << dp[j];
    }
    HERE(INFO);
    LOG(INFO) << "tmax:" << tmax << " max:" << max;
    */
  }
  return max;
}
void Read() {
  while (scanf("%d%d", &L, &W) != EOF) {
    getchar();
    v.clear();
    scanf("%d", &N);
    getchar();
    for (int i = 0; i < N; i++) {
      int x, y;
      scanf("%d%d", &x, &y);
      getchar();
      if (v.count(x) == 0) v[x] = std::set<int>();
      v[x].insert(y);
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
