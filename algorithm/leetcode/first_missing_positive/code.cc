// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 10:12:54
// File  : code.cc
// Brief :

/*
Given an unsorted integer array, find the first missing positive integer.

For example,
Given [1,2,0] return 3,
and [3,4,-1,1] return 2.

Your algorithm should run in O(n) time and uses constant space.

必然从1开始

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 23/23 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 20 milli secs
 * Progress: 156/156 test cases passed.
 *
 * */

#include <algorithm>
#include "base/public/logging.h"

namespace algorithm {
int FirstMissingPositive(int * a, int n) {
  for (int i = 0; i < n; i++) {
    if (a[i] <= n && a[i] > 0) {
      if (a[i] != i + 1 && a[i] != a[a[i] - 1]) {
        // 没有判断的话会造成死循环 e.g a[1] = 2
        // 没有a[i] != a[a[i] - 1] 在{1, 1} 时会出现死循环
        std::swap(a[i], a[a[i] - 1]);
        i--;  // 因为当前swap过来的可能是一个正数
      }
    }
  }
  for (int i = 0; i < n; i++) {
    if (a[i] != i + 1) return i + 1;
  }
  return n + 1;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  int A[] = {3, 4, -1, 1};
  int size = sizeof(A) / sizeof(int);
  LOG(INFO) << FirstMissingPositive(A, size);
  return 0;
}
