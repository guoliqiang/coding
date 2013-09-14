// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 20:27:41
// File  : pick_diff_two.h
// Brief :

#ifndef  __PICK_DIFF_TWO_H_
#define  __PICK_DIFF_TWO_H_

#include <vector>

namespace algorithm {

void PickDiffTwo(std::vector<int> & v, int * d1, int * d2) {
  int foo = v[0];
  for (int i = 1; i < v.size(); i++) {
   foo ^= v[i];
  }
  int index = 0;
  while ((foo & (1 << index)) == 0) index++;
  *d1 = 0;
  *d2 = 0;
  for (int i = 0; i < v.size(); i++) {
    if (v[i] & 1 << index) *d1 ^= v[i];
    else *d2 ^= v[i];
  }
}

}  // namespace algorithm


#endif  // __PICK_DIFF_TWO_H_
