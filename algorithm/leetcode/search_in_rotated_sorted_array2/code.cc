// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 21:03:08
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 110/110 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 48 milli secs
 * Progress: 271/271 test cases passed.
 *
 * */

/*
 * 先判断mid的位置，然后判断t在有序一侧的条件
 * 然后进入else的部分就是t在旋转有序的一侧
 * */

#include "base/public/common_head.h"

namespace algorithm {
bool BSearch (int * a, int b, int e, int t) {
  if (b > e) return false;
  if (b == e) return a[b] == t;
  
  int mid = b + (e - b) / 2;
  if (a[mid] == t) return true;
  if (a[mid] > a[e]) {
     if (t > a[e] && t < a[mid])
       return BSearch(a, b, mid - 1, t);
     else
       return BSearch(a, mid + 1, e, t);
  } else if (a[mid] < a[e]) {
    if (t <= a[e] && t > a[mid])
      return BSearch(a, mid + 1, e, t);
     else
       return BSearch(a, b, mid - 1, t);
  } else {
    return BSearch(a, b, e - 1, t);
  }
}

bool BSearch(int * a, int n, int t) {
  return BSearch(a, 0, n - 1, t);
}

}  // namespace algorithm


using namespace algorithm;

int main(int argc, char** argv) {
  int A[] = {3,3,3,3,4,5,6,7,0,1,2,3,3,3,3};
  int size = sizeof(A) / sizeof(int);
  LOG(INFO) << BSearch(A, size, 5);
  LOG(INFO) << BSearch(A, size, 10);
  LOG(INFO) << BSearch(A, size, 3);
  LOG(INFO) << BSearch(A, size, 0);
  return 0;
}
