// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-17 21:34:29
// File  : code.cc
// Brief :
#include "code.h"
#include "../k-th_smallest_of_two_sorted_array/k-th.h"
using namespace algorithm;

int main(int argc, char** argv) {
  int a[] = {1, 2, 2.6};
  int b[] = {3, 4, 5, 6};
  int size = arraysize(a) + arraysize(b);
  if (size % 2 == 0) {
    double foo = (FindKthSmallestWrapper(a, arraysize(a), b, arraysize(b), size/2 + 1) +
                  FindKthSmallestWrapper(a, arraysize(a), b, arraysize(b), size/2)) / 2.0;
    LOG(INFO) << FindKthSmallestWrapper(a, arraysize(a), b, arraysize(b), size/2);
    LOG(INFO) << foo;
  } else {
    LOG(INFO) << FindKthSmallestWrapper(a, arraysize(a), b, arraysize(b), size/2 + 1);
  }
  // LOG(INFO) << FindKthSmallestWrapper(a, 2, b, 4, 3);
  LOG(INFO) << findMedianSortedArrays(a, arraysize(a), b, arraysize(b));
  return 0;
}
