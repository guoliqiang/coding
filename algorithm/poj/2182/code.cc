// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-19 01:12:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace NB {
// Accepted  196K  47MS
const int MAXN = 8002;
int N = 0;
int ta[MAXN];
int data[MAXN];

// 返回k末尾0个数的2此幂
int Low(int k) {
  return k & (k ^ (k - 1));
}

int Sum(int k) {
  int rs = 0;
  for (int i = k; i > 0; i -= Low(i)) {
    rs += ta[i];
  }
  return rs;
}

void Add(int k) {
  for (int i = k; i <= N; i += Low(i)) {
    ta[i] += 1;
  }
}

int BSearch(int k) {
  int b = 1;
  int e = N;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (mid - Sum(mid) < k) b = mid + 1;
    else e = mid - 1;
  }
  return b;
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF) {
    getchar();
    N = n;
    memset(ta, 0, sizeof(ta));
    memset(data, 0, sizeof(data));
    for (int i = 2; i <= n; i++) {
      int t = 0;
      scanf("%d", &t);
      getchar();
      data[i] = t;
    }
    for (int i = N ; i >= 1; i--) {
      int t = BSearch(data[i] + 1);
      data[i] = t;
      Add(t);
    }
    for (int i = 1; i <= N; i++) {
      printf("%d\n", data[i]);
    }
  }

}

}

namespace algorithm {
/*
 * Accepted  228K  344MS
 * */
const int MAXN = 8002;
int N = 0;
int flag[MAXN];
int in[MAXN];
int out[MAXN];

int Find(int k) {
  int num = 0;
  // 求flag数组中i之前的1的个数，可以用树状数组，如NB中的
  for (int i = 1; i <= N; i++) {
    if (flag[i] == 0) num++;
    if (num == k) return i;
  }
  return 0;
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF) {
    getchar();
    N = n;
    memset(flag, 0, sizeof(flag));
    memset(in, 0, sizeof(in));
    memset(out, 0, sizeof(out));
    for (int i = 2; i <= n; i++) {
      int t = 0;
      scanf("%d", &t);
      getchar();
      in[i] = t;
    }
    for (int i = N ; i >= 1; i--) {
      out[i] = Find(in[i] + 1);
      flag[out[i]] = 1;
    }
    for (int i = 1; i <= N; i++) {
      printf("%d\n", out[i]);
    }
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  NB::Read();
  return 0;
}
