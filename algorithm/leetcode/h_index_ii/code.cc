// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 14:50:18
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int HIndex(std::vector<int>& citations) {
  int cnt = 1;
  int max = 0;
  for (int i = citations.size() - 1; i >= 0; i--) {
    if (citations[i] >= cnt) max = cnt;
    cnt++;
  }
  return max;
}
}  // namespace algorithm

using namespace algorithm;

namespace NB {
int hIndex(std::vector<int>& citations) {
  int n = citations.size();
  int b = 0;
  int e = n - 1;

  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (citations[mid] >= n - mid) e = mid - 1;
    else b = mid + 1;
  }
  return n - b;
}
}  // namespace NB

namespace my {
int hIndex(std::vector<int>& citations) {
  int n = citations.size();
  if (n == 0) return 0;

  int b = 0;
  int e = n - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (mid == b) {
      if (citations[mid] >= n - mid) return n - mid;
      else b = mid + 1;
    } else {
      if (citations[mid] >= n - mid && citations[mid - 1] < n - (mid - 1)) return n - mid;
      else if (citations[mid] >= n - mid) e = mid - 1;
      else b = mid + 1;
    }
  }
  return 0;
}
}  // namespace my

int main(int argc, char** argv) {
  return 0;
}
