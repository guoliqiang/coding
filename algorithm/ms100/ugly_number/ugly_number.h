// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 15:07:57
// File  : ugly_number.h
// Brief :

#ifndef  __UGLY_NUMBER_H_
#define  __UGLY_NUMBER_H_

#include "base/public/logging.h"
#include <stdlib.h>
#include <algorithm>

namespace algorithm {

int ugly_number(int k) {
  std::vector<int> foo;
  foo.push_back(1);
  int index_2 = 0;
  int index_3 = 0;
  int index_5 = 0;
  while (foo.size() < k) {
    int bar2 = foo[index_2] * 2;
    int bar3 = foo[index_3] * 3;
    int bar5 = foo[index_5] * 5;
    int bar_min = std::min(bar2, std::min(bar3, bar5));
    foo.push_back(bar_min);
    if (bar2 == bar_min) index_2++;
    if (bar3 == bar_min) index_3++;
    if (bar5 == bar_min) index_5++;
  }
  return foo.back();
}

}  // namespace ugly_number
#endif  // __UGLY_NUMBER_H_
