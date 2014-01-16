// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-14 23:17:51
// File  : code.cc
// Brief :

// Accepted 2680K 438MS

#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 50009;
int N;
int max;
int min;

int begin[MAXN];
int end[MAXN];
int count[MAXN];
int interval[MAXN];

bool Cmp(int x, int y) {
  return end[x] < end[y] || (end[x] == end[y] && begin[x] < begin[y]);
}

struct Node {
  int b;
  int e;
  int cnt;
  Node(int bi = 0, int ei = 0) : b(bi), e(ei), cnt(0) {}
} data[MAXN * 3];  // multiply 2 wrong why?

void Build(int b, int e, int k) {
  if (b <= e) {
    // LOG(INFO) << b << "~" << e << " at " << k << "(" << min << "," << max << ")";
    data[k].b = b;
    data[k].e = e;
    data[k].cnt = 0;
    if (b == e) return;
    
    int mid = b + (e - b) / 2;
    Build(b, mid, 2 * k);
    Build(mid + 1, e, 2 * k + 1);
  }
}

int Count(int b, int e, int k) {
  if (data[k].b >= b && data[k].e <= e) return data[k].cnt;
  else {
    int mid = data[k].b + (data[k].e - data[k].b) / 2;
    if (mid >= e) return Count(b, e, 2 * k);
    else if (mid < b) return Count(b, e, 2 * k + 1);
    else return Count(b, mid, 2 * k) + Count(mid + 1, e, 2 * k + 1);
  }
}

bool Insert(int v, int k) {
  if (data[k].b == v && data[k].e == v) {
    if (data[k].cnt == 1) return false;
    int t = k;
    while (t >= 1) {
      data[t].cnt++;
      t /= 2;
    }
    return true;
  } else {
    int mid = data[k].b + (data[k].e - data[k].b) / 2;
    if (v <= mid) return Insert(v, 2 * k);
    else return Insert(v, 2 * k + 1);
  }
}

int Solve() {
  std::sort(interval, interval + N, Cmp);
  Build(min, max, 1);
  for (int i = 0; i < N; i++) {
    int idx = interval[i];
    int b = begin[idx];
    int e = end[idx];
    int t = Count(b, e, 1);
    
    if (t < count[idx]) {
      t = count[idx] - t;
      while (t) {
        if (Insert(e, 1)) t--;
        e--;
      }
    }
  }
  return data[1].cnt;
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    memset(begin, 0, sizeof(begin));
    memset(end, 0, sizeof(end));
    memset(count, 0, sizeof(count));
    memset(data, 0, sizeof(data));
    memset(interval, 0, sizeof(interval));
    max = -1;
    min = INF;
    
    for (int i = 0; i < N; i++) {
      interval[i] = i;
      scanf("%d%d%d", &(begin[i]), &(end[i]), &(count[i]));
      getchar();
      max = std::max(end[i], max);
      min = std::min(begin[i], min);
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
