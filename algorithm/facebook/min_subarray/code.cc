// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-01 12:00:24
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int Min(std::vector<int> & v, int t) {
  int rs = -1;
  int n = v.size();
  int b = 0;
  int e = 0;
  int sum = 0;
  while (e < n) {
    sum += v[e];
    if (sum >= t) {
      while (b <= e) {
        if (sum - v[b] >= t) {
          sum -= v[b++];
        } else break;
      }
      rs = rs == -1 ? e - b + 1 : std::min(rs, e - b + 1);
    }
    e++;
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(2);
  v.push_back(1);
  v.push_back(1);
  v.push_back(1);
  v.push_back(1);
  LOG(INFO) << Min(v, 4);
  return 0;
}
