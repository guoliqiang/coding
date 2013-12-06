// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-02 23:31:31
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int data[MAX] = {0};

struct Node {
  int val;
  int last;
  int first;
  int num;
  int b; // debug
  int e; // debug
} rmq[MAX];

void Build(int index, int b, int e) {
  rmq[index].b = b;
  rmq[index].e = e;
  if (b == e) {
    rmq[index].val = data[b];
    rmq[index].num = 1;
    rmq[index].last = 1;
    rmq[index].first = 1;
  } else {
    int mid = b + (e - b) / 2;
    Build(2 * index, b, mid);
    Build(2 * index + 1, mid + 1, e);
    if (rmq[2 * index].num > rmq[2 * index + 1].num) {
      rmq[index].num = rmq[2 * index].num;
      rmq[index].val = rmq[2 * index].val;
    } else {
      rmq[index].num = rmq[2 * index + 1].num;
      rmq[index].val = rmq[2 * index + 1].val;
    }
    if (data[mid] == data[mid + 1]) {
      int tb = mid;
      int te = mid + 1;
      while (tb >= b && data[tb] == data[mid]) tb--;
      while (te <= e && data[te] == data[mid + 1]) te++;
      if (te - tb - 1 > rmq[index].num) {
        rmq[index].num = te - tb - 1;
        rmq[index].val = data[mid];
      }
      if (rmq[2 * index].first == mid - b + 1) {
        rmq[index].first = rmq[2 * index].first + rmq[2 * index + 1].first;
      } else {
        rmq[index].first = rmq[2 * index].first;
      }
      if (rmq[2 * index + 1].last == e - mid) {
        rmq[index].last = rmq[2 * index + 1].last + rmq[2 * index].last;
      } else {
        rmq[index].last = rmq[2 * index + 1].last;
      }
    } else {
      rmq[index].first = rmq[2 * index].first;
      rmq[index].last = rmq[2 * index + 1].last;
    }
  }
}

// 注意要把整个数组的起始与结束位置传入进来，计算mid的时候，用的是
// 这两个值
std::pair<int, int> Query(int index, int b, int e, int qb, int qe) {
  if (qb == b && qe == e) {
    return std::make_pair(rmq[index].val, rmq[index].num);
  } else {
    if (qe < b || qb > e) return std::make_pair(0, 0);
    int mid = b + (e - b) / 2;
    if (qe <= mid) return Query(2 * index, b, mid, qb, qe);
    if (qb > mid) return Query(2 * index + 1, mid + 1, e, qb, qe);
    std::pair<int, int> tl = Query(2 * index, b, mid, qb, mid);
    std::pair<int, int> tr = Query(2 * index + 1, mid + 1, e, mid + 1, qe);
    int max = std::max(tl.second, tr.second);
    if (data[mid] != data[mid + 1]) return tl.second > tr.second ? tl : tr;
    if (rmq[2 * index].last + rmq[2 * index + 1].first > max) {
      return std::make_pair(data[mid], rmq[2 * index].last + rmq[2 * index + 1].first);
    } else {
      return tl.second > tr.second ? tl : tr;
    }
  }
}

void Read(std::vector<int> & v) {
  std::sort(v.begin(), v.end());
  N = v.size();
  memset(data, 0, sizeof(data));
  for (int i = 0; i < v.size() + 1; i++) {
    data[i + 1] = v[i];
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(-1);
  v.push_back(-1);
  v.push_back(1);
  v.push_back(1);
  v.push_back(1);
  v.push_back(1);
  v.push_back(3);
  v.push_back(10);
  v.push_back(10);
  v.push_back(10);
  Read(v);
  Build(1, 1, N);
  int qb = 1;
  int qe = 7;
  std::pair<int, int> t = Query(1, 1, N, qb, qe);
  LOG(INFO) << t.first << " " << t.second;
  qb = 7;
  qe = 7;
  t = Query(1, 1, N, qb, qe);
  LOG(INFO) << t.first << " " << t.second;
  return 0;
}
