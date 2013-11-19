// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-19 02:18:24
// File  : code.cc
// Brief :

/*
 * Accepted  7960K  532MS
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 500002;
int N = 0;
long long data[MAXN];
long long cdata[MAXN];

long long Merge(int b, int e) {
  if (b >= e) return 0;
  long long rs = 0;
  int mid = b + (e - b) / 2;
  rs += (Merge(b, mid) + Merge(mid + 1, e));
  int k = 0;
  int i = b;
  int j = mid + 1;
  while (i <= mid && j <= e) {
    if (data[i] > data[j]) {
      rs += (mid - i + 1);
      cdata[k++] = data[j++];
    } else cdata[k++] = data[i++];
  }
  while (i <= mid) cdata[k++] = data[i++];
  while (j <= e) cdata[k++] = data[j++];
  for (i = b; i <= e; i++) data[i] = cdata[i - b];
  return rs;
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF && n) {
    getchar();
    N = n;
    memset(data, 0, sizeof(data));
    memset(cdata, 0, sizeof(data));
    for (int i = 0; i < n; i++) {
      long long t = 0;
      scanf("%lld", &t);
      getchar();
      data[i] = t;
    }
    long long t = Merge(0, N - 1);
    printf("%lld\n", t);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
