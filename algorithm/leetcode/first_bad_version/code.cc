// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 14:57:00
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool isBadVersion(int version);

int FirstBadVersion(int n) {
  int b = 1;
  int e = n;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    bool state = isBadVersion(mid);
    if (state) {
      if (isBadVersion(mid - 1) == false) return mid;
      else e = mid - 1;
    } else {
      b = mid + 1;
    }
  }
  return 0;
}

int FirstBadVersion2(int n) {
  int b = 1;
  int e = n;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (isBadVersion(mid)) e = mid - 1;
    else b = mid + 1;
  }
  return b;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
