// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-25 07:10:02
// File  : code.cc
// Brief :

// Accepted 204K  16MS

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 10000;
int N;
int dp[MAXN];

int BSearch(int t) {
  int b = 0;
  int e = N - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (dp[mid] < t && (mid == 0 || dp[mid - 1] >= t)) return mid;
    else if (dp[mid] < t) e = mid - 1;
    else b = mid + 1;
  }
  return b;
}

void Read() {
  int t = 0;
  int cnt = 0;
  while (scanf("%d", &t) != EOF && t != -1) {
    getchar();
    N = 0;
    cnt++;
    memset(dp, 0, sizeof(dp));
    do {
      int idx = BSearch(t);
      dp[idx] = t;
      if (idx == N) N++;
      scanf("%d", &t);
      getchar();
    } while (t != -1);
    printf("Test #%d:\n", cnt);
    printf("  maximum possible interceptions: %d\n\n", N);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
