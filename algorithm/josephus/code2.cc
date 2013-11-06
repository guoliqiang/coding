// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-05 17:53:23
// File  : code2.cc
// Brief :

/*
 * 使用线段树模拟，时间负载度n*log(n)
 *
 * */
#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
struct Node {
  int left;
  int right;
  int num;
};
Node data[MAX];

int Build(int k, int l, int r) {
  data[k].left = l;
  data[k].right = r;
  data[k].num = 0;
  if (l == r) {
    data[k].num = 1;
  } else {
    int mid = l + (r - l) / 2;
    data[k].num += Build(2 * k, l, mid);
    data[k].num += Build(2*k + 1, mid + 1, r);
  }
  return data[k].num;
}

void Build(int n) {
  Build(1, 1, n);
}

void Update(int k) {
  // LOG(INFO) << k;
  if (k <= 0) return;
  data[k].num--;
  Update(k / 2);
}

int Find(int root, int k) {
  if (k > data[root].num) return -1;
  if (data[root].left == data[root].right) return root;
  if (data[2 * root].num >= k) return Find(2 * root, k);
  else return Find(2 * root + 1, k - data[2 * root].num);
}

void Josephus(int n, int m) {
  Build(n);
  int seq = 1;
  for (int i = 0; i < n; i++) {
    seq = (seq + m - 1) % data[1].num;
    if (seq == 0) seq = data[1].num;
    int t = Find(1, seq);
    LOG(INFO) << data[t].left;
    Update(t);
  }
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 10;
  int m = 2;
  Josephus(n, m);
  return 0;
}
