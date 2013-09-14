// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-13 22:59:29
// File  : p47.h
// Brief :

#ifndef  __P47_H_
#define  __P47_H_

#include <vector>
#include "base/public/logging.h"

namespace algorithm {
template <typename type>
type MoreHalfNum(std::vector<type> & v) {
  int count = 1;
  type foo = v[0];
  for (int i = 1; i < v.size(); i++) {
    if (v[i] == foo) count++;
    if (v[i] != foo) count--;
    if (count == 0) {
      foo = v[i];
      count = 1;
    }
  }
  return foo;
}
}  // namespace algorithm

#endif  // __P47_H_
