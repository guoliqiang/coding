// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-03 04:13:05
// File  : code.cc
// Brief :

/*
注意:
基于快速排序的算法时间复杂度是O(n)

*/
#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int data[MAX] = {0};

int Kth(int k, int b, int e) {
  if (k > e) return -1;
  int mid = b + (e - b) / 2;
  std::swap(data[mid], data[e]);
  int t = -1;
  for (int i = b; i < e; i++) {
    if (data[i] < data[e]) std::swap(data[i], data[++t]);
  }
  std::swap(data[++t], data[e]);
  if (t == k) return data[t];
  else {
    if (t > k) return Kth(k, b, mid);
    else {
      return Kth(k - t - 1, mid + 1, e);
    }
  }
}

int Kth(int k) {
  return Kth(k - 1, 0, N - 1);
}

void Read(std::vector<int> & v) {
  memset(data, 0, sizeof(data));
  N = v.size();
  for (int i = 0; i < N; i++) {
    data[i] = v[i];
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(10);
  v.push_back(3);
  v.push_back(5);
  v.push_back(1);
  Read(v);
  LOG(INFO) << Kth(1);
  LOG(INFO) << Kth(3);
  return 0;
}
