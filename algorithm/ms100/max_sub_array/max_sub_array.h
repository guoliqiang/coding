// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 16:29:41
// File  : max_sub_tree.h
// Brief :

#ifndef  __MAX_SUB_TREE_H_
#define  __MAX_SUB_TREE_H_

#include <vector>
#include "base/public/logging.h"

namespace algorithm {
template<typename type>
int MaxSubArray(std::vector<type> & a, int * begin = NULL,
                int * length = NULL) {
  CHECK_GT(a.size(), 0);
  std::vector<type> foo;
  std::vector<type> len;
  foo.assign(a.size(), 0);
  len.assign(a.size(), 0);
  foo[0] = a[0];
  len[0] = 1;
  type max = foo[0];
  int max_index = 0;
  for (int i = 1; i < a.size(); i++) {
    if (a[i] + foo[i - 1] > a[i]) {
      foo[i] = a[i] + foo[i - 1];
      len[i] = len[i - 1] + 1;
    }
    else {
      foo[i] = a[i];
      len [i] = 1;
    }
    if (foo[i] > max) {
      max = foo[i];
      max_index = i;
    }
  }
  if (begin != NULL) {
    *begin = max_index - len[max_index] + 1;
  }
  if (length != NULL) *length = len[max_index];
  return max;
}
}  // namspace algorithm

#endif  // __MAX_SUB_TREE_H_
