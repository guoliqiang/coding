// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-26 19:31:03
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 105;
int M;

struct Node {
  int b;
  int p;
} dp[MAXN][MAXN];

int B[MAXN][MAXN];
int P[MAXN][MAXN];
int L[MAXN];

struct MNode {
  int max;
  int min;
  int smax;
  int smin;
} TB[MAXN], TP[MAXN];

void Build() {
  memset(TB, 0, sizeof(TB));
  memset(TP, 0, sizeof(TP));
  for (int i = M - 1; i >= 0; i--) {
    int tbmax = 0;
    int tbmin = INF;
    int tpmax = 0;
    int tpmin = INF;
    for (int j = 0; j < L[i]; j++) {
      tbmax = std::max(tbmax, B[i][j]);
      tbmin = std::min(tbmin, B[i][j]);
      tpmax = std::max(tpmax, P[i][j]);
      tpmin = std::min(tpmin, P[i][j]);
    }
    if (i == M - 1) {
      TB[i].max = tbmax;
      TB[i].min = tbmin;
      TP[i].max = tpmax;
      TP[i].min = tpmin;

      TB[i].smax = tbmax;
      TB[i].smin = tbmin;
      TP[i].smax = tpmax;
      TP[i].smin = tpmin;
    } else {
      TB[i].max = std::max(tbmax, TB[i - 1].max);
      TB[i].min = std::min(tbmin, TB[i - 1].min);
      TP[i].max = std::max(tpmax, TP[i - 1].max);
      TP[i].min = std::min(tpmin, TP[i - 1].min);
      
      TB[i].smax = tbmax + TB[i - 1].max;
      TB[i].smin = tbmin + TB[i - 1].min;
      TP[i].smax = tpmax + TP[i - 1].max;
      TP[i].smin = tpmin + TP[i - 1].min;
    }
  }
}

void Trace(int k, int minb, int sump, double & rs) {
  if (k >= M) {
    rs = std::max(rs, double(minb) / double(sump));
  } else {
    double tb = std::min(minb, TB[k].max);
    double tp = sump + (M - k) * TP[k].min;
    if (tb / tp < rs) return;
    tp = sump + TP[k].smin;
    if (tb / tp < rs) return;
    for (int i = 0; i < L[k]; i++) {
      Trace(k + 1, std::min(minb, B[k][i]), sump + P[k][i], rs);
    }
  }
}

double Trace() {
  double rs = 0;
  Trace(0, INF, 0, rs);
  return rs;
}

double Dp() {
  memset(dp, 0, sizeof(dp));
  for (int i = 0; i < L[0]; i++) {
    dp[0][i].b = B[0][i];
    dp[0][i].p = P[0][i];
  }

  for (int i = 1; i < M; i++) {
    for (int j = 0; j < L[i]; j++) {
      int tb = 0;
      int tp = 0;
      double tv = 0;
      for (int k = 0; k < L[i - 1]; k++) {
        double minb = std::min(B[i][j], dp[i - 1][k].b);
        double sump = dp[i - 1][k].p + P[i][j];
        if (minb / sump > tv) {
          tb = minb;
          tp = sump;
          tv = minb / sump;
        }
      }
      dp[i][j].b = tb;
      dp[i][j].p = tp;
    }
  }
  double rs = 0;
  for (int i = 0; i < L[M - 1]; i++) {
    rs = std::max(rs, double(dp[M - 1][i].b) / double(dp[M - 1][i].p));
  }
  return rs;
}

void Read() {
  int c = 0;
  scanf("%d", &c);
  for (int k = 0; k < c; k++) {
    memset(B, 0, sizeof(B));
    memset(P, 0, sizeof(P));
    memset(L, 0, sizeof(L));
    scanf("%d", &M);
    for (int i = 0; i < M; i++) {
      scanf("%d", &(L[i]));
      for (int j = 0; j < L[i]; j++) {
        scanf("%d%d", &(B[i][j]), &(P[i][j]));
      }
    }
    Build();
    // double t = Dp();
    double t = Trace();
    printf("%.3f\n", t);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
