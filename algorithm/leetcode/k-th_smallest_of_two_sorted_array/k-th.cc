// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-17 16:56:34
// File  : k-th.cc
// Brief :
#include "k-th.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int32_t> a;
  std::vector<int32_t> b;
  for (int i = 0; i < 10; i++) {
    a.push_back(i);
  }
  for (int i = 10; i < 11; i++) {
    b.push_back(i);
  }
  for (int i = 0; i < 11; i++) {
    LOG(INFO) << FindKthSmallest(a, b, i + 1, kint32max, kint32min);
  }
  return 0;
}
