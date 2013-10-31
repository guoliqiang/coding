// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-31 05:10:09
// File  : code.cc
// Brief :

#include "base/public/common_head.h"
#include "../base/base.h"

namespace algorithm {

const int MAX = 1000;
Point vec[MAX];
int index[MAX];
int temp[MAX];


bool Cmp(int a, int b) {
  return (vec[a].x < vec[b].x) ||
         (vec[a].x == vec[b].x && vec[a].y < vec[b].y);
}

// 0~ m-1  m ~ n
void Merge(int * v, int n, int m) {
  memset(temp, 0 , sizeof(temp));
  int k = 0;
  
  int i = n;
  int j = m - 1;
  while (i >= m && j >= 0) {
    if (vec[v[i]].y > vec[v[j]].y) {
      temp[k++] = v[i];
      i--;
    } else {
      temp[k++] = v[j];
      j--;
    }
    // LOG(INFO) << "tmp[" << k -1 << "]:" << temp[k - 1];
  }
  while (i >= m) {
    temp[k++] = v[i];
    i--;
  }
  for (int l = k - 1; l >= 0; l--) {
    v[++j] = temp[l];
  }
  HERE(INFO);
  for (int i = 0; i <= n; i++) {
    LOG(INFO) << v[i] << ":[" << vec[v[i]].x << "," << vec[v[i]].y << "]";
  }
  HERE(INFO);
}

double Closest(int * v, int n) {
  if (n <= 0) {
    return 0;
  }
  if (n == 1) {
    int t = Distance(vec[v[0]], vec[v[1]]);
    if (vec[v[0]].y > vec[v[1]].y) std::swap(v[0], v[1]);
    return t;
  }
  if (n == 2) {
    double t = Distance(vec[v[0]], vec[v[1]]);
    t = std::min(t, Distance(vec[v[1]], vec[v[2]]));
    t = std::min(t, Distance(vec[v[0]], vec[v[2]]));
    // 冒泡排序
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        if (vec[v[j]].y > vec[v[j + 1]].y) std::swap(v[j], v[j + 1]);
      }
    }
    return t;
  }
  int mid = (n + 1) / 2;
  double dml = Closest(v, mid - 1);
  double dmr = Closest(v + mid, n - mid);
  double dm = std::min(dml, dmr);
  // LOG(INFO) << "left:" << dml << " right:" << dmr << " min:" << dm;
  Merge(v, n, mid);
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n && vec[v[j]].y - vec[v[i]].y < dm; j++) {
      dm = std::min(dm, Distance(vec[v[j]], vec[v[i]]));
    }
  }
  return dm;
}

double Read(std::vector<Point> & v) {
  memset(index, 0, sizeof(index));
  for (int i = 0; i < v.size(); i++) {
    vec[i] = v[i];
    index[i] = i;
  }
  int * ptr = index;
  std::sort(ptr, ptr + v.size(), Cmp);
  for (int i = 0; i < v.size(); i++) {
    LOG(INFO) << "[" << vec[ptr[i]].x << "," << vec[ptr[i]].y << "]";
  }
  return Closest(ptr, v.size() - 1);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<Point> v;
  v.push_back(Point(1, 1.5));
  v.push_back(Point(6, 0.5));
  v.push_back(Point(0.5, 6.5));
  v.push_back(Point(4, 8.5));
  v.push_back(Point(2, 5));
  v.push_back(Point(-2, 5));
  v.push_back(Point(10, 5));
  v.push_back(Point(21, -9));
  v.push_back(Point(-1, 4));
  v.push_back(Point(3, 8));
  v.push_back(Point(4, -3));
  v.push_back(Point(12, -2));
  v.push_back(Point(7, -5));
  v.push_back(Point(0, 0));
  LOG(INFO) << Read(v);
  return 0;
}
