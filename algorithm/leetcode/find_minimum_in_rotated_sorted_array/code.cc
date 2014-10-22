// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-11-21 02:44:39
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int FindMin(std::vector<int> & v) {
  int n = v.size();
  if (n == 1) return v[0];
  if (v[0] < v[1] && v[0] < v[n - 1]) return v[0];
  if (v[n - 1] < v[0] && v[n - 1] < v[n - 2]) return v[n - 1];
  int b = 0;
  int e = n - 1;
  while (b < e) {
    int mid = b + (e - b) / 2;
    // LOG(INFO) << b << " " << e << " " << mid;
    if (v[mid] < v[mid + 1] && v[mid] < v[mid - 1]) return v[mid];
    else {
      if (v[mid] > v[e]) b = mid + 1;
      else e = mid - 1;
    }
  }
  return v[b];
}

// 146 / 146 test cases passed.
// Status: Accepted
// Runtime: 52 ms
int FindMin2(std::vector<int> & v) {
  int n = v.size();
  int b = 0;
  int e = n - 1;
  while (b < e) {
    int mid = b + (e - b) / 2;
    if ((mid == b && v[mid] < v[mid + 1]) ||
        (mid == e && v[mid] < v[mid - 1]) ||
        (v[mid] < v[mid + 1] && v[mid] < v[mid - 1])) return v[mid];
    else {
      if (v[mid] > v[e]) b = mid + 1;
      else e = mid - 1;
    }
  }
  return v[b];
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(0);
  v.push_back(5);
  v.push_back(6);
  v.push_back(7);
  LOG(INFO) << FindMin(v);
}
