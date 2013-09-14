// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 20:09:17
// File  : p42.h
// Brief :

#ifndef  __P42_H_
#define  __P42_H_

#include <vector>
#include "base/public/logging.h"

namespace algorithm {
template <typename type>
type MinOfRotateArraySub(std::vector<type> & a, int i, int j) {
  if (i == j - 1) return a[i];
  else {
    int mid = i + (j - i) / 2;
    // Note: this line is important, use much time to fix...
    if (a[mid] < a[mid -1]) return a[mid];
    if (a[mid] > a.back()) {
      return MinOfRotateArraySub(a, mid, j);
    } else {
      return MinOfRotateArraySub(a, i, mid);
    }
  }
}

template <typename type>
type MinOfRotateArray(std::vector<type> & a) {
  if (a[0] != a[a.size() - 1]) {
    return MinOfRotateArraySub(a, 0, a.size());
  } else {
    int rs = a[0];
    for (int i = 1; i < a.size(); i++) {
      if(a[i] < a[i - 1]) rs = a[i];
    }
    return rs;
  }
  return 0;
}

}  // namespace algorithm

#endif  //__P42_H_
