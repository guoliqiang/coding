// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-04 17:43:02
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

int LIS(std::vector<int> v) {
  int size = 0;
  for (int i = 0; i < v.size(); i++) {
    int b = 0;
    int e = size - 1;
    while (b <= e) {
      int mid = b + (e - b) / 2;
      if (v[mid] < v[i]) b = mid + 1;
      else e = mid - 1;
    }
    v[b] = v[i];
    if (b == size) size++;
  }
  return size;
}

std::vector<int> GetRs(std::vector<int> v, int t) {
  int size = 0;
  std::vector<int> rs;
  for (int i = 0; i < v.size(); i++) {
    int b = 0;
    int e = size - 1;
    while (b <= e) {
      int mid = b + (e - b) / 2;
      if (v[mid] < v[i]) b = mid + 1;
      else e = mid - 1;
    }
    v[b] = v[i];
    if (b == size) size++;
    if (size == t && rs.size() == 0) rs = std::vector<int>(v.begin(), v.begin() + size);
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> v;
  // 1 2 3 8 10 5 6 7 12 9 4 0
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(8);
  
  v.push_back(10);
  v.push_back(5);
  v.push_back(6);
  v.push_back(7);
  
  v.push_back(12);
  v.push_back(9);
  v.push_back(4);
  v.push_back(0);
  int size = LIS(v);
  LOG(INFO) << JoinVector(v);
  LOG(INFO) << JoinVector(GetRs(v, size));
  return 0;
}
