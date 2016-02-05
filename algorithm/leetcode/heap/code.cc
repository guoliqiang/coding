// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-21 12:09:27
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

void ReBuild(std::vector<int> & vec, int idx) {
  int size = vec.size();
  while (idx * 2 + 1 < size) {
    int max = idx * 2 + 1;
    if (idx * 2 + 2 < size) {
      max = vec[idx * 2 + 1] > vec[idx * 2 + 2] ? idx * 2 + 1 : idx * 2 + 2;
    }
    if (vec[idx] < vec[max]) {
      std::swap(vec[idx], vec[max]);
      idx = max;
    } else break;
  }
}

// 在已经建立好堆的vector上，更新中间的某个元素
void ReBuildRandom(std::vector<int> & vec, int idx) {
  while (true) {
    ReBuild(vec, idx);
    if (idx == 0) break;
    idx = (idx - 1) / 2;
  }
}

void PushHeap(std::vector<int> & vec, int v) {
  vec.push_back(v);
  int idx = vec.size() - 1;
  while (idx != 0) {
    int parent = (idx - 1) / 2;
    if (vec[idx] > vec[parent]) {
      std::swap(vec[idx], vec[parent]);
      idx = parent;
    } else break;
  }
}

void Build(std::vector<int> & vec) {
  int n = vec.size();
  for (int i = n / 2; i >= 0; i--) {
    ReBuild(vec, i);
  }
}

std::vector<int> HeapSort(std::vector<int> vec) {
  int n = vec.size();
  std::vector<int> ans;

  Build(vec);
  for (int i = 0; i < n; i++) {
    ans.push_back(vec[0]);
    std::swap(vec[0], vec[vec.size() - 1]);
    vec.pop_back();
    ReBuild(vec, 0);
  }
  return ans;
}

std::vector<int> TopK(std::vector<int> & vec, int k) {
  std::vector<int> ans;

  int n = vec.size();
  for (int i = 0; i < k; i++) {
    ans.push_back(vec[0]);
    std::swap(vec[0], vec[n - 1]);

    int idx = 0;
    while (idx * 2 + 1 < n) {
      int max = idx * 2 + 1;
      if (idx * 2 + 2 < n) {
        max = vec[idx * 2 + 1] > vec[idx * 2 + 2] ? idx * 2 + 1 : idx * 2 + 2;
      }
      if (vec[idx] < vec[max]) {
        std::swap(vec[idx], vec[max]);
        idx = max;
      } else break;
    }
    n--;
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(20);
  vec.push_back(15);
  vec.push_back(19);
  vec.push_back(10);
  vec.push_back(11);
  vec.push_back(18);
  vec.push_back(16);
  vec.push_back(3);
  vec.push_back(5);
  vec.push_back(8);
  vec.push_back(9);
  vec.push_back(13);
  vec.push_back(14);
  vec.push_back(12);
  vec.push_back(11);
  LOG(INFO) << JoinVector(HeapSort(vec));
  LOG(INFO) << JoinVector(TopK(vec, 3));
  return 0;
}
