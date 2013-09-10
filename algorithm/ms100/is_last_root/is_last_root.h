// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 17:48:44
// File  : is_last_root.h
// Brief :

#ifndef  __IS_LAST_ROOT_H_
#define  __IS_LAST_ROOT_H_

#include <vector>
#include "base/public/logging.h"

namespace algorithm {
template <typename type>
bool IsLastRootSub(std::vector<type> & a, int i, int j) {
  if (i >= j || i == j - 1 ) return true;
  CHECK(j <= a.size());
  int index = j;
  for (int k = i; k < j; k++) {
    if (a[k] > a[j - 1]) {
      index = k;
      break;
    }
  }
  for (int k = index; k < j; k++) {
    if (a[k] < a[j-1]) return false;
  }
  if (index == j) {
    return  IsLastRootSub(a, i, j - 1);
  } else {
    return IsLastRootSub(a, i, index) && IsLastRootSub(a, index, j - 1);
  }
}

template <typename type>
bool IsLastRoot(std::vector<type> & a) {
  return IsLastRootSub(a, 0, a.size());
}

}  // namespace algorithm
#endif  // __IS_LAST_ROOT_H_
