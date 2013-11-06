// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-05 19:58:38
// File  : code.cc
// Brief :

/*
 * 线段树
 * Accepted 3700K 360MS
 * */
#include "base/public/common_head.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <algorithm>

namespace algorithm {

const int MAX = 150000;  // 这是叶子节点的个数，树节点的个数至少是其两倍
struct Node {
  int left;
  int right;
  int color;
  Node(int c = 1) : left(0), right(0), color(c){}
};
Node data[MAX * 3];  // note  MAX * 3

int count[35] = {0};

void Build(int k, int l, int r) {
  data[k].left = l;
  data[k].right = r;
  if (r == l) return;
  int mid = l + (r - l) / 2;
  Build(2 * k, l, mid);
  Build(2 * k + 1, mid + 1, r);
}

void Build(int n) {
  Build(1, 1, n);
}

void Insert(int k, int l, int r, int c) {
  // LOG(INFO) << "k:" << k << " l:" << l << " r:" << r;
  // LOG(INFO) << data[k].left  << "~" << data[k].right;
  if (data[k].left >= l && data[k].right <= r) {
    data[k].color = c;
  } else {
    if (data[k].color > 0) {
      data[2 * k].color = data[k].color;
      data[2 * k + 1].color = data[k].color;
      if (data[k].color != c) data[k].color = -1;
    }
    int mid = data[k].left + (data[k].right - data[k].left) / 2;
    if (mid >= r) Insert(2 * k, l, r, c);
    else if (mid < l) Insert(2 * k + 1, l, r, c);
    else {
      Insert(2 * k, l, mid, c);
      Insert(2 * k + 1, mid + 1, r, c);
    }
  }
}

void Insert(int l, int r, int c) {
  Insert(1, l, r, c);
}

void Count(int k, int l, int r) {
  // LOG(INFO) << "k:" << k << " l:" << l << " r:" << r;
  // LOG(INFO) << data[k].left  << "~" << data[k].right << " " << data[k].color;
  if (data[k].color != -1) {
    count[data[k].color]++;
  } else {
    int mid = data[k].left + (data[k].right - data[k].left) / 2;
    if (mid >= r) Count(2 * k, l, r);
    else if (mid < l) Count(2 * k + 1, l, r);
    else {
      Count(2 * k, l, mid);
      Count(2 * k + 1, mid + 1, r);
    }
  }
}

int Count(int l, int r) {
  memset(count, 0, sizeof(count));
  Count(1, l, r);
  int rs = 0;
  for (int i = 0; i < sizeof(count) / sizeof(int); i++) {
    if (count[i] > 0) rs++;
  }
  return rs;
}

void Read() {
  freopen("test.txt", "r", stdin);
  int L = 0;
  int T = 0;
  int O = 0;
  scanf("%d%d%d", &L, &T, &O);
  getchar();
  Build(L);
  for (int i = 0; i < O; i++) {
    char ch = '\0';
    int l = 0;
    int r = 0;
    int c = 0;
    scanf("%c", &ch);
    if (ch == 'C') {
      scanf("%d%d%d", &l, &r, &c);
      if (l > r) std::swap(l, r);
      Insert(l, r, c);
    } else {
      scanf("%d%d", &l, &r);
      if (l > r) std::swap(l, r);
      printf("%d\n", Count(l, r));
    }
    getchar();
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  Read();
  return 0;
}
