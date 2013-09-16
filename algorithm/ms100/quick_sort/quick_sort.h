// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-16 00:44:42
// File  : quit_sort.h
// Brief :

#ifndef  __QUIT_SORT_H_
#define  __QUIT_SORT_H_

#include <string>
#include <vector>
#include <algorithm>
#include "stdlib.h"
#include "time.h"
#include "base/public/logging.h"

namespace algorithm {

template <typename type>
void Qsort(std::vector<type> & v, int b, int e) {
  if (b >= e) return;
  type foo = v[e];
  int i = b;
  int j = e - 1;
  while (i < j) {
    while (i < e && v[i] < foo) i++;
    while (j > b && v[j] >= foo) j--;
    if (i < j) {
      std::swap(v[i], v[j]);
    }
  }
  std::swap(v[i], v[e]);
  Qsort(v, b, i - 1);
  Qsort(v, i + 1, e);
}

template <typename type>
void Qsort(std::vector<type> & v) {
  if (v.size() == 0) return;
  Qsort(v, 0, v.size() - 1);
}

}  // namespace algorithm

#endif  //  __QUIT_SORT_H_
