// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 14:17:23
// File  : code.cc
// Brief :

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
