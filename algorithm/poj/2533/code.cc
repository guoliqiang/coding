// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-19 03:53:34
// File  : code.cc
// Brief :

/*
 * Accepted  136K  0MS
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1009;
int N = 0;
int data[MAXN];
// O(n * log(n))
int LCI() {
  int t = 0;
  for (int i = 0; i < N; i++) {
    int b = 0;
    int e = t;
    while (b <= e) {
      int mid = b + (e - b) / 2;
      if (data[mid] < data[i]) b = mid + 1;
      else e = mid - 1;
    }
    data[b] = data[i];
    t = std::max(t, b);
  }
  return t + 1;
}

// O(n^2)
int dp[MAXN];
int LCI2() {
  int rs = 0;
  memset(dp, 0, sizeof(dp));
  for (int i = 0; i < N; i++) {
    if (i == 0) dp[i] = 1;
    else {
      int max = 0;
      for (int j = 0; j < i; j++) {
        if (data[j] < data[i]) max = std::max(max, dp[j]);
      }
      dp[i] = max + 1;
    }
    rs = std::max(rs, dp[i]);
  }
  return rs;
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF) {
    N = n;
    memset(data, 0, sizeof(data));
    getchar();
    for (int i = 0; i < n; i++) {
      int t = 0;
      scanf("%d", &t);
      data[i] = t;
    }
    getchar();
    int t = LCI();
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
