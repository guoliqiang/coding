// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-02-03 21:51:31
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int FindMax(std::vector<int> & vec) {
  int b = 0;
  int e = vec.size() - 1;
  while (b < e) {
    if (vec[b] < vec[e]) return vec[e];
    int mid = b + (e - b + 1) / 2;  // 不加1会死循环
    if (vec[mid] > vec[e]) b = mid;
    else e = mid - 1;
  }
  return vec[b];
}

int FindMaxDup(std::vector<int> & vec) {
  int b = 0;
  int e = vec.size() - 1;
  while (b < e) {
    if (vec[b] < vec[e]) return vec[e];
    else if (vec[b] == vec[e]) b++;
    else {
      int mid = b + (e - b + 1) / 2;
      if (vec[mid] > vec[e]) b = mid;
      else if (vec[mid] < vec[e]) e = mid - 1;
      else e--;
    }
  }
  return vec[b];
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(5);
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(1);
  vec.push_back(1);
  vec.push_back(1);
  LOG(INFO) << FindMaxDup(vec);
  return 0;
}
