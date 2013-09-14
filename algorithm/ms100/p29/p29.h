// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 16:33:50
// File  : p29.h
// Brief :

#ifndef  __P29_H_
#define  __P29_H_

#include <vector>
#include "base/public/logging.h"
#include "base/public/string_util.h"



namespace algorithm {

void swap(int * a, int * b) {
  int foo = *a;
  *a = *b;
  *b = foo;
}

void SwapOddEven(std::vector<int> & i) {
  int b = 0;
  int e = i.size() - 1;
  while (b <= e) {
    while (i[b] % 2 == 1 && b < e) b++;
    while (i[e] % 2 == 0 && e > b) e--;
    if (b >= e) break;
    swap(&i[b], &i[e]);
  }
}

}  // namespace algorithm

#endif  //__P29_H_
