// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 14:29:27
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

int HIndex(std::vector<int>& v) {
  int max = 0;
  std::sort(v.begin(), v.end(), std::greater<int>());
  for (int i = 0; i < v.size(); i++) {
    if (v[i] >= i + 1) max = i + 1;
  }
  return max;
}

}  // namespace algorithm

namespace NB {
int HIndex(std::vector<int>& v) {
  int n = v.size();
  std::vector<int> count(n + 1, 0);
  for (int i = 0; i < n; i++) {
    if (v[i] >= n) count[n]++;
    else count[v[i]]++;
  }

  int total = 0;
  for (int i = n; i >= 1; i--) {
    total += count[i];
    if (total >= i) {
      return i;
    }
  }
  return 0;
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(3);
  vec.push_back(0);
  vec.push_back(6);
  vec.push_back(1);
  vec.push_back(5);
  LOG(INFO) << HIndex(vec);
  LOG(INFO) << NB::HIndex(vec);
  return 0;
}
