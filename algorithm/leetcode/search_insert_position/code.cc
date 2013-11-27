// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 15:11:50
// File  : code.cc
// Brief :

/*
Given a sorted array and a target value, return the index if the target is found.
If not, return the index where it would be if it were inserted in order.
You may assume no duplicates in the array.

Here are few examples.
[1,3,5,6], 5 → 2
[1,3,5,6], 2 → 1
[1,3,5,6], 7 → 4
[1,3,5,6], 0 → 0

*/

#include "base/public/logging.h"

namespace algorithm {

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 19/19 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 40 milli secs
 * Progress: 62/62 test cases passed.
 *
 * */

int SearchBigger(int * a, int n, int t) {
  int b = 0;
  int e = n;
  if (b >= e) return -1;
  while (b < e) {
    int mid = b + (e - b) / 2;
    if ((a[mid] > t && (mid == 0 || a[mid - 1] < t)) ||
        a[mid] == t) {
      return mid;
    } else {
      if (a[mid] > t) e = mid;
      else b = mid + 1;
    }
  }
  // all smaller than t
  return n;
}

}  // namespace algorithm

namespace twice {

int BSearch2(int A[], int n, int t) {
  int b = 0;
  int e = n - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (A[mid] < t) b = mid + 1;
    else e = mid - 1;
  }
  return b;
}

int BSearch(int A[], int n, int t) {
  int b = 0;
  int e = n;
  while (b < e) {
    int mid = b + (e - b) / 2;
    if (A[mid] < t) b = mid + 1;
    else e = mid;
  }
  return b;
}

class Solution {
 public:
  int searchInsert(int A[], int n, int target) {
    // Note: The Solution object is instantiated only once and is reused by each test case.
    return BSearch(A, n, target);
  }
};
}  // namespace twice

using namespace algorithm;


int main(int argc, char** argv) {
  int A[] = {1,3,5,6};
  int size = sizeof(A) / sizeof(int);
  LOG(INFO) << SearchBigger(A, size, 5);
  LOG(INFO) << SearchBigger(A, size, 2);
  LOG(INFO) << SearchBigger(A, size, 7);
  LOG(INFO) << SearchBigger(A, size, 0);
  

  LOG(INFO) << SearchBigger(A, size, 1);
  LOG(INFO) << SearchBigger(A, size, 3);
  LOG(INFO) << SearchBigger(A, size, 5);
  LOG(INFO) << SearchBigger(A, size, 6);
  return 0;
}
