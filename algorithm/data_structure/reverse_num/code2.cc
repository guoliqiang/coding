// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-06 02:16:21
// File  : code2.cc
// Brief :

/*
 * 使用线段数统计
 * */
#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
struct DNode {
  int val;
  int idx;
} data[MAX];

struct Node {
  int l;
  int r;
  int num;
} node[MAX * 3];

struct SNode {
  int val;
  int id;
} sdata[MAX];

bool Cmp(const SNode & x, const SNode & y) {
  return x.val < y.val;
}

void Build(int k, int l, int r) {
  node[k].l = l;
  node[k].r = r;
  node[k].num = 0;
  if (l == r) return;
  int mid = l + (r - l) / 2;
  Build(2 * k, l, mid);
  Build(2 * k + 1, mid + 1, r);
}

int Query(int k, int l, int r) {
  if (node[k].l >= l && node[k].r <= r) return node[k].num;
  else {
    int mid = node[k].l + (node[k].r - node[k].l) / 2;
    if (mid >= r) return Query(2 * k, l, r);
    else if (mid < l) return Query(2 * k + 1, l, r);
    else {
      return Query(2 * k, l, mid) + Query(2 * k + 1, mid + 1, r);
    }
  }
}

void Update(int k, int v) {
  if (node[k].l <= v && node[k].r >= v) node[k].num++;
  if (node[k].l == node[k].r) return;
  int mid = node[k].l + (node[k].r - node[k].l) / 2;
  if (v <= mid) Update(2 * k, v);
  else Update(2 * k + 1, v);
}

int Read(std::vector<int> & v) {
  // 离散化
  for (int i = 0; i < v.size(); i++) {
    data[i].val = v[i];
    data[i].idx = -1;
    sdata[i].val = v[i];
    sdata[i].id = i;
  }
  SNode * ptr = sdata;
  std::sort(ptr, ptr + v.size(), Cmp);
  int val = - 1;
  int k = 0;
  for (int i = 0; i < v.size(); i++) {
    if (k == 0 || val != ptr[i].val) {
      k++;
      val = ptr[i].val;
    };
    data[ptr[i].id].idx = k;
  }
  Build(1, 1, k);
  int rs = 0;
  for (int i = 0; i < v.size(); i++) {
    int t = data[i].idx;
    if (t < k) rs += Query(1, t + 1, k);  // 统计t+1 ～ k数的个数
    Update(1, t);
  }
  return rs;
}


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(100);
  v.push_back(150);
  v.push_back(10);
  v.push_back(300);
  LOG(INFO) << Read(v);
  return 0;
}
