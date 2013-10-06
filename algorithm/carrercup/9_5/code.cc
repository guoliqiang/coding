// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 22:04:21
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

int BSearch(std::vector<std::string> & v, const std::string & k) {
  int b = 0;
  int e = v.size();
  if (b == e) return -1;
  while (b < e) {
    int mid = b + (e - b) / 2;
    int t = mid;
    while (mid >= b && v[mid] == "") mid--;
    if (mid < b) {
      b = t + 1;
    } else {
      if (v[mid] == k) return mid;
      if (v[mid] < k) b = mid + 1;
      else e = mid;
    }
  }
  return -1;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::string> foo;
  foo.push_back("at");
  foo.push_back("");
  foo.push_back("");
  foo.push_back("");
  foo.push_back("ball");
  foo.push_back("");
  foo.push_back("");
  foo.push_back("car");
  foo.push_back("");
  foo.push_back("");
  foo.push_back("dad");
  foo.push_back("");
  foo.push_back("");
  std::string k = "ball";
  LOG(INFO) << BSearch(foo, k);
  LOG(INFO) << BSearch(foo, "ballcar");
  return 0;
}

