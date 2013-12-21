// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-22 00:14:13
// File  : code.cc
// Brief :

// http://love-oriented.com/pack/  背包九讲
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 202;
int N;
const int MAXV = 200005;
int V;
int dp[MAXV];
int v[MAXN];
int w[MAXN];
int m[MAXN];

void ZeroPack(int cost, int val) {
  for (int j = V; j >= cost; j--) {
    dp[j] = std::max(dp[j], dp[j - cost] + val);
  }
}

void ComplatePack(int cost, int val) {
  for (int j = cost; j <= V; j++) {
    dp[j] = std::max(dp[j], dp[j - cost] + val);
  }
}

void MultiPack(int cost, int val, int n)  {
  if (cost * n >= V) {
    ComplatePack(cost, val);
  } else {
    int k = 1;
    while (k < n) {
      ZeroPack(cost * k, val * k);
      n -= k;
      k *= 2;
    }
    ZeroPack(cost * n, val * n);
  }
}

int DpNB() {
  memset(dp, 0, sizeof(dp));
  for (int i = 1; i <= N; i++) {
    if (m[i] == -1) {
      ComplatePack(v[i], w[i]);
    } else if (m[i] == 1) {
      ZeroPack(v[i], w[i]);
    } else {
      MultiPack(v[i], w[i], m[i]);
    }
  }
  return dp[V];
}

int Dp() {
  memset(dp, 0, sizeof(dp));
  for (int i = 1; i <= N; i++) {
    if (m[i] == -1) {
      for (int j = v[i]; j <= V; j++) {
        dp[j] = std::max(dp[j], dp[j - v[i]] + w[i]);
      }
    } else {
      for (int j = V; j >= v[i]; j--) {
        int t = j / v[i];
        for (int k = 1; k <= std::min(m[i], t); k++) {
          dp[j] = std::max(dp[j], dp[j - v[i] * k] + k * w[i]);
        }
      }
    }
  }
  return dp[V];
}

void Read() {
  while (scanf("%d%d", &N, &V) != EOF) {
    getchar();
    for (int i = 1; i <= N; i++) {
      int vi, wi, mi;
      scanf("%d%d%d", &vi, &wi, &mi);
      getchar();
      v[i] = vi;
      w[i] = wi;
      m[i] = mi;
    }
    int t = DpNB();
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
