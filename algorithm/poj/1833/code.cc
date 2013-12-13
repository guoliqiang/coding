// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-09 18:21:32
// File  : code.cc
// Brief :

/*
Accepted  168K  485MS
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1025;
int N = 0;
int data[MAXN];

void Next() {
  int j = N - 1;
  while (j - 1 >= 0 && data[j - 1] > data[j]) j--;
  if (j == 0) {
    int b = 0;
    int e = N - 1;
    while (b < e) {
      std::swap(data[b], data[e]);
      b++;
      e--;
    }
  } else {
    int i = N - 1;
    while (data[i] < data[j - 1]) i--;
    std::swap(data[j - 1], data[i]);
    int b = j;
    int e = N - 1;
    while (b < e) {
      std::swap(data[b], data[e]);
      b++;
      e--;
    }
  }
}

void Read() {
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int i = 0; i < c; i++) {
    int n, k;
    scanf("%d%d", &n, &k);
    getchar();
    N = n;
    for (int j = 0; j < n; j++) {
      int t;
      scanf("%d", &t);
      data[j] = t;
    }
    getchar();
    while (k--) Next();
    for (int j = 0; j < n; j++) printf("%d ", data[j]);
    printf("\n");
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
