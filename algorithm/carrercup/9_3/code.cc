// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 21:31:56
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

int BSearch(std::vector<int> & a, int k) {
  int b = 0;
  int e = a.size();
  if (b == e) return -1;
  while (b < e) {
    int mid = b + (e - b) / 2;
    if (a[mid] == k) return mid;
    if (a[mid] > a[e - 1]) {
      if (k > a[e - 1] && k < a[mid]) e = mid;
      else b = mid + 1;
    } else {
      if (k <= a[e - 1] && k > a[mid]) b = mid + 1;
      else e = mid;
    }
  }
  return -1;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(15);
  foo.push_back(16);
  foo.push_back(19);
  foo.push_back(20);
  foo.push_back(25);
  foo.push_back(1);
  foo.push_back(3);
  foo.push_back(4);
  foo.push_back(5);
  foo.push_back(7);
  foo.push_back(10);
  foo.push_back(14);
  LOG(INFO) << BSearch(foo, 5);
  LOG(INFO) << BSearch(foo, 2);
  return 0;
}
