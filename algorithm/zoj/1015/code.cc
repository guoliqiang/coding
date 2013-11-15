// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-14 19:20:40
// File  : code.cc
// Brief :

/*
 * Accepted
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1010;
int N = 0;
int matrix[MAXN][MAXN];
int order[MAXN];
int num[MAXN];
int val[MAXN];

void MSC() {
  memset(order, -1, sizeof(order));
  memset(val, -1, sizeof(val));
  memset(num, 0, sizeof(num));
  for (int i = N - 1; i >= 0; i--) {
    int j = 0;
    while (order[j] >= 0) j++;
    for (int k = j + 1; k < N; k++) {
      if (order[k] < 0 && num[k] > num[j]) j = k;
    }
    order[j] = i;
    
    val[i] = j;
    for (int k = 0; k < N; k++) {
      if (matrix[j][k] > 0) num[k]++;
    }
  }
}

bool Check() {
  for (int i = N -2; i >= 0; i--) {
    int min = N;
    int u = val[i];
    for (int j = i + 1; j < N; j++) {
      int v = val[j];
      if (matrix[u][v] > 0) {
        min = v;
        break;
      }
    }
    if (min == N) continue;
    for (int j = i + 1; j < N; j++) {
      int v = val[j];
      if (v == min || matrix[u][v] == 0) continue;
      if (matrix[min][v] == 0) return false;
    }
  }
  return true;
}

void Read() {
  int n, m;
  while (scanf("%d%d", &n, &m) && (n + m)) {
    getchar();
    N = n;
    memset(matrix, 0, sizeof(matrix));
    for (int i = 0; i < m; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      getchar();
      matrix[u - 1][v - 1] = 1;
      matrix[v - 1][u - 1] = 1;
    }
    MSC();
    if (Check()) printf("Perfect\n\n");
    else printf("Imperfect\n\n");
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
