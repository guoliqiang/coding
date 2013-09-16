// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-16 23:09:56
// File  : p61.h
// Brief :

#ifndef  __P61_H_
#define  __P61_H_

#include "../max_sub_array/max_sub_array.h"
#include "base/public/logging.h"

namespace algorithm {
int MaxDiff(std::vector<int> & v) {
  int min = v[v.size() - 1];
  int maxdiff = 0;
  for (int i = v.size() - 2; i >= 0; i--) {
    int foo = 0;
    if (v[i] > min) foo = v[i] - min;
    if (maxdiff < foo) maxdiff = foo;
    if (v[i] < min) min = v[i];
  }
  return maxdiff;
}

int MaxDiff2(std::vector<int> & v) {
  std::vector<int> foo;
  for(int i = 0; i < v.size() - 1; i++) {
    foo.push_back(v[i] - v[i + 1]);
  }
  foo.push_back(0);
  return MaxSubArray(foo);
}
}  // namespace algorithm


#endif  //  __P61_H_
