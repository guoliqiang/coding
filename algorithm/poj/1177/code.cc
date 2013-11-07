// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-06 04:58:54
// File  : code.cc
// Brief :

/*
 *  线段树求矩形并的周长
 *  http://www.cnblogs.com/Booble/archive/2010/10/10/1847163.html
 *  Accepted  776K  250MS
 *
 * */


#include "base/public/common_head.h"
// #include "base/public/common_ojchead.h"
// #include <map>

namespace algorithm {
const int MAX = 5000;

std::map<int, int> v_id;
std::map<int, int> id_v;
int rdata[MAX * 4];

struct DNode {
  int val;
  int small;
  int big;
  bool add;
};

bool Cmp(const DNode & x, const DNode & y) {
  return x.val < y.val;
}
DNode ydata[MAX * 2];
DNode xdata[MAX * 2];

struct Node {
  int l;
  int r;
  int cover;
};

Node data[MAX * 3];

void Build(int k, int l, int r) {
  if (l == r) return;
  data[k].l = l;
  data[k].r = r;
  data[k].cover = 0;
  if (l + 1== r) return;
  int mid = l + (r - l) / 2;
  Build(2 * k, l, mid);
  Build(2 * k + 1, mid, r);
}

void Insert(int k, int l, int r) {
  if (r == l) return;
  if (data[k].l >= l && data[k].r <= r) {
    data[k].cover++;
  } else {
    int mid = data[k].l + (data[k].r - data[k].l) / 2;
    if (mid >= r) Insert(2 * k, l, r);
    else if (mid <= l) Insert(2 * k + 1, l, r);
    else {
      Insert(2 * k, l, mid);
      Insert(2 * k + 1, mid, r);
    }
  }
}


void Delete(int k, int l, int r) {
  if (l == r) return;
  if (data[k].l >= l && data[k].r <= r) {
    data[k].cover--;
  } else {
    int mid = data[k].l + (data[k].r - data[k].l) / 2;
    if (mid >= r) Delete(2 * k, l, r);
    else if (mid <= l) Delete(2 * k + 1, l, r);
    else {
      Delete(2 * k, l, mid);
      Delete(2 * k + 1, mid, r);
    }
  }
}

int GetSum(int k) {
  if (data[k].cover > 0) {
    // LOG(INFO) << "sum add:" << data[k].l << "[" << id_v[data[k].l] << "] "
    // << data[k].r << " [" << id_v[data[k].r] << "]";
    return id_v[data[k].r] - id_v[data[k].l];
  } else {
    if (data[k].l + 1== data[k].r) return 0;
    return GetSum(2 * k) + GetSum(2 * k + 1);
  }
}

int One(int n, int m, DNode * ptr) {
  memset(data, 0, sizeof(data));
  // LOG(INFO) << "build:1 ~ " << m;
  Build(1, 1, m);
  int sum = 0;
  int pre = 0;
  for (int i = 0; i < n * 2; i++) {
    int l = v_id[ptr[i].small];
    int r = v_id[ptr[i].big];
    if (ptr[i].add) {
      Insert(1, l, r);
      // LOG(INFO) << "insert " << l << "[" << ptr[i].small << "] "
      // << r << "[" << ptr[i].big << "]";
    }
    else {
      Delete(1, l, r);
      // LOG(INFO) << "delete " << l << "[" << ptr[i].small << "] "
      // << r << "[" << ptr[i].big << "]";
    }
    int t = GetSum(1);
    // LOG(INFO) << "get sum:" << t;
    sum += abs(t - pre);
    pre = t;
  }
  return sum;
}


int Read() {
  int n = 0;
  freopen("test.txt", "r", stdin);
  scanf("%d", &n);
  getchar();
  for (int i = 0; i < n; i++) {
    int x1, y1, x2, y2;
    scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
    rdata[i * 4 + 0] = x1;
    rdata[i * 4 + 1] = y1;
    rdata[i * 4 + 2] = x2;
    rdata[i * 4 + 3] = y2;
    xdata[i * 2 + 0].val = x1;
    xdata[i * 2 + 0].small = y1;
    xdata[i * 2 + 0].big = y2;
    xdata[i * 2 + 0].add = true;

    xdata[i * 2 + 1].val = x2;
    xdata[i * 2 + 1].small = y1;
    xdata[i * 2 + 1].big = y2;
    xdata[i * 2 + 1].add = false;

    ydata[i * 2 + 0].val = y1;
    ydata[i * 2 + 0].small = x1;
    ydata[i * 2 + 0].big = x2;
    ydata[i * 2 + 0].add = true;

    ydata[i * 2 + 1].val = y2;
    ydata[i * 2 + 1].small = x1;
    ydata[i * 2 + 1].big = x2;
    ydata[i * 2 + 1].add = false;
    getchar();
  }
  std::sort(xdata, xdata + n * 2, Cmp);
  std::sort(ydata, ydata + n * 2, Cmp);
  std::sort(rdata, rdata + n * 4);
  int k = 0;
  int val = 0;
  for (int i = 0; i < n * 4; i++) {
    if (k == 0 || rdata[i] != val) {
      k++;
      val = rdata[i];
      v_id.insert(std::make_pair(rdata[i], k));
      id_v.insert(std::make_pair(k, rdata[i]));
    }
  }
  int rs = 0;
  rs += One(n, k, xdata);
  rs += One(n, k, ydata);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Read();
  return 0;
}
