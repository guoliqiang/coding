// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 14:17:23
// File  : code.cc
// Brief :

/*
Given a sorted array of integers, find the starting and ending position of a given target value.
Your algorithm's runtime complexity must be in the order of O(log n).
If the target is not found in the array, return [-1, -1].

For example,
Given [5, 7, 7, 8, 8, 10] and target value 8,
return [3, 4].

广义二分查找

*/

#include <algorithm>
#include <vector>
#include "base/public/logging.h"

/*
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 18/18 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 80 milli secs
 * Progress: 81/81 test cases passed.
 *
 * */

namespace algorithm {

bool BSF(int * a, int n, int t, int * index) {
  int b = 0;
  int e = n;

  if (b >= e) return false;
  while (b < e) {
    int mid = b + (e - b) / 2;
    if (a[mid] == t && (mid == 0 || a[mid - 1] != t)) {
      *index = mid;
      return true;
    } else {
      if (a[mid] >= t) e = mid;
      else b = mid + 1;
    }
  }
  return false;
}

bool BSL(int * a, int n, int t, int * index) {
  int b = 0;
  int e = n;

  if (b >= e) return false;
  while (b < e) {
    int mid = b + (e - b) / 2;
    if (a[mid] == t && (mid == n - 1 || a[mid + 1] != t)) {
      *index = mid;
      return true;
    } else {
      if (a[mid] <= t) b = mid + 1;
      else e = mid;
    }
  }
  return false;
}

std::vector<int> SearchRange(int * a, int n, int t) {
  int first = -1;
  int last = -1;
  std::vector<int> rs;

  if (BSF(a, n, t, &first)) {
    BSL(a, n, t, &last);
  }
  LOG(INFO) << "first:" << first << " last:" << last;
  rs.push_back(first);
  rs.push_back(last);
  return rs;
}

}  // namespace algorithm

namespace twice {
std::vector<int> SearchRange(int A[], int n, int target) {
  std::vector<int> rs(2, -1);
  int b, e;
  b = 0;
  e = n - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (A[mid] == target && (mid == 0 || A[mid - 1] < target)) {
      rs[0] = mid;
      break;
    } else if (A[mid] >= target) e = mid - 1;
    else b = mid + 1;
  }
  b = 0;
  e = n - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (A[mid] == target && (mid == e || A[mid + 1] > target)) {  // mid == n 也可以，但mid == e 更有实际意义
      rs[1] = mid;
      break;
    } else if (A[mid] <= target) b = mid + 1;
    else e = mid - 1;
  }
  return rs;
}
}  // namespace twice
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> rs;
  int A[] = {5, 7, 7, 8, 8, 10};
  int size = sizeof(A) / sizeof(int);
  SearchRange(A, size, 8);
  SearchRange(A, size, 5);
  SearchRange(A, size, 10);
  SearchRange(A, size, 11);
  return 0;
}
