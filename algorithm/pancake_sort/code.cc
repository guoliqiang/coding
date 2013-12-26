// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-24 14:44:55
// File  : code.cc
// Brief :

// 煎饼排序 pancake sorting
#include "base/public/common_ojhead.h"

namespace algorithm {
void Flip(std::vector<int> & v, int k) {
  int b = 0;
  int e = k;
  while (b < e) std::swap(v[b++], v[e--]);
}

void PSort(std::vector<int> & v) {
  int n = v.size();
  for (int i = n - 1; i >= 0; i--) {
    int max = i;
    for (int j = i - 1; i >= 0; j--) {
      if (v[j] > v[max]) max = j;
    }
    if (max == i) continue;
    Flip(v, max);
    Flip(v, i);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
