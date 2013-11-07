// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-06 05:00:23
// File  : code.cc
// Brief :

/*
 * 线段树求矩形并的面积。
 * WA
 * http://wenku.baidu.com/link?url=j05jLbF89y0fyVfw7Vi1RYd5GoWfoc5MK
 * Aq8piZCBMhZcLsQo-pqt2GsCvnsGPPRkFjdiPb2Jh6He4DwEHD0A1scSgiMPjxt-AiiBBr0XWm
 * http://blog.csdn.net/youngyangyang04/article/details/7787693
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 100;
std::map<float, int> v_id;
std::map<int, float> id_v;
double rdata[MAX * 2];

struct DNode {
 float val;
 float small;
 float big;
 bool add;
};
DNode xdata[MAX * 2];
bool Cmp(const DNode & x, const DNode & y) {
  return x.val < y.val;
}

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
  if (l + 1 == r) return;
  int mid = l + (r - l) / 2;
  Build(2 * k, l, mid);
  Build(2 * k + 1, mid, r);
}

void Insert(int k, int l, int r) {
  if (l == r) return;
  // LOG(INFO) << "Insert " << l << " " << r
  //           << " with data " << data[k].l << " " << data[k].r;
  if (data[k].l >= l && data[k].r <= r) {
    data[k].cover++;
  } else {
    if (data[k].l + 1 == data[k].r) return;
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
    if (data[k].l + 1 == data[k].r) return;
    int mid = data[k].l + (data[k].r - data[k].l) / 2;
    if (mid >= r) Delete(2 * k, l, r);
    else if (mid <= l) Delete(2 * k + 1, l, r);
    else {
      Delete(2 * k, l, mid);
      Delete(2 * k + 1, mid, r);
    }
  }
}

float GetSum(int k) {
  if (data[k].cover > 0) {
    // LOG(INFO) << "sum " << data[k].l << " " << data[k].r;
    return id_v[data[k].r] - id_v[data[k].l];
  } else {
    if (data[k].l + 1 == data[k].r) return 0;
    return GetSum(2 * k) + GetSum(2 * k + 1);
  }
}

float One(int n, int m) {
  memset(data, 0, sizeof(data));
  Build(1, 1, m);
  float rs = 0;
  for (int i = 0; i < 2 * n; i++) {
    int l = v_id[xdata[i].small];
    int r = v_id[xdata[i].big];
    if (xdata[i].add) {
      // LOG(INFO) << "add " << l << "-" << xdata[i].small << " " << r << "-" << xdata[i].big;
      Insert(1, l, r);
    } else {
      // LOG(INFO) << "delete " << l << "-" << xdata[i].small << " " << r << "-" << xdata[i].big;
      Delete(1, l, r);
    }
    float t = GetSum(1);
    // LOG(INFO) << "get sum:" << t;
    if (i + 1 < 2 * n) rs += t * (xdata[i + 1].val - xdata[i].val);
  }
  return rs;
}

void Read() {
  freopen("test.txt", "r", stdin);
  int num = 1;
  int n = 0;
  while (scanf("%d", &n) != EOF && n != 0) {
    getchar();
    v_id.clear();
    id_v.clear();
    memset(rdata, 0, sizeof(rdata));
    memset(xdata, 0, sizeof(xdata));
    for (int i = 0; i < n; i++) {
      float x1, y1, x2, y2;
      scanf("%f%f%f%f", &x1, &y1, &x2, &y2);
      rdata[i * 2 + 0] = y1;
      rdata[i * 2 + 1] = y2;

      xdata[i * 2 + 0].val = x1;
      xdata[i * 2 + 0].small = y1;
      xdata[i * 2 + 0].big = y2;
      xdata[i * 2 + 0].add = true;
      
      xdata[i * 2 + 1].val = x2;
      xdata[i * 2 + 1].small = y1;
      xdata[i * 2 + 1].big = y2;
      xdata[i * 2 + 1].add = false;
      getchar();
    }
    std::sort(rdata, rdata + 2 * n);
    std::sort(xdata, xdata + 2 * n, Cmp);
    int k = 0;
    float val = -1;
    for (int i = 0; i < 2 * n; i++) {
      if (k == 0 || rdata[i] != val) {
        k++;
        val = rdata[i];
        id_v.insert(std::make_pair(k, rdata[i]));
        v_id.insert(std::make_pair(rdata[i], k));
        // LOG(INFO) << "[" << k << "]:" << rdata[i];
      }
    }
    float t = One(n, k);
    printf("Test case #%d\nTotal explored area: %.2lf\n\n", num++, t);
    // LOG(INFO) << t;
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  Read();
  return 0;
}
