// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-20 15:14:00
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 100005;
const int mo = 99999997;
int N;
struct Node {
  int v;
  int idx;
} a[MAXN], b[MAXN];
int c[MAXN];
int temp[MAXN];

bool Cmp(const Node & x, const Node & y) {
  return x.v < y.v;
}

int Merge(int b, int e) {
  if (b >= e) return 0;
  int mid = b + (e - b) / 2;
  int rs = Merge(b, mid) + Merge(mid + 1, e);
  int i = b;
  int j = mid + 1;
  int k = b;
  while (i <= mid || j <= e) {
    if (i > mid) temp[k++] = c[j++];
    else if (j > e) {
      rs = (rs + j - mid - 1) % mo;
      temp[k++] = c[i++];
    } else if (c[i] < c[j]) {
      rs = (rs + j - mid - 1) % mo;
      temp[k++] = c[i++];
    } else {
      temp[k++] = c[j++];
    }
  }
  for (i = b; i <= e; i++) c[i] = temp[i];
  return rs;
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    for (int i = 0; i < N; i++) {
      scanf("%d", &(a[i].v));
      a[i].idx = i;
    }
    getchar();
    for (int i = 0; i < N; i++) {
      scanf("%d", &(b[i].v));
      b[i].idx = i;
    }
    std::sort(a, a + N, Cmp);
    std::sort(b, b + N, Cmp);
    for (int i = 0; i < N; i++) {
      c[a[i].idx] = b[i].idx;
    }
    int t = Merge(0, N - 1);
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
