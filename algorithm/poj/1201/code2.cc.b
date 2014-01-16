// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-14 23:17:51
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// TLE
namespace algorithm {

const int MAXN = 50009;
int N;
int max;
int b[MAXN];
int e[MAXN];
int c[MAXN];
int interval[MAXN];
int result[MAXN];

bool Cmp(int x, int y) {
  return e[x] < e[y] || (e[x] == e[y] && b[x] < b[y]);
}

int OverLoop(int k) {
  int rs = 0;
  for (int i = b[k]; i <= e[k]; i++) {
    if (result[i] == 1) rs++;
  }
  return rs;
}

void Set(int k, int t) {
  for (int i = e[k]; i >= b[k] && t; i--) {
    if (result[i] == 0) {
      result[i] = 1;
      t--;
    }
  }
}

int Solve() {
  memset(result, 0, sizeof(result));
  std::sort(interval, interval + N, Cmp);
  for (int i = 0; i < N; i++) {
    int idx = interval[i];
    int t = OverLoop(idx);
    if (t < c[idx]) {
      Set(idx, c[idx] - t);
    }
  }
  int rs = 0;
  for (int i = 0; i <= max; i++) {
    if (result[i] == 1) rs++;
  }
  return rs;
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    memset(b, 0, sizeof(b));
    memset(e, 0, sizeof(e));
    memset(c, 0, sizeof(c));
    max = -1;
    memset(interval, 0, sizeof(interval));
    for (int i = 0; i < N; i++) {
      interval[i] = i;
      scanf("%d%d%d", &(b[i]), &(e[i]), &(c[i]));
      getchar();
      max = std::max(e[i], max);
    }
    int t = Solve();
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
