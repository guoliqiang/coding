// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 22:15:44
// File  : one_num.h
// Brief :

#ifndef  __ONE_NUM_H_
#define  __ONE_NUM_H_

#include "base/public/logging.h"

namespace algorithm {

int onenum(int i) {
  int rs = 0;
  while (i != 0) { // worng i > 0
   rs++;
   i &= i-1;
  }
  return rs;
}

bool ispow(int i) {
  return (i & (i-1)) == 0 ? true : false;
}

// O(1)
int onenum2(uint64_t i) {
  uint64_t a = 0x5555555555555555;
  uint64_t b = 0x3333333333333333;
  uint64_t c = 0x0f0f0f0f0f0f0f0f;
  uint64_t d = 0x00ff00ff00ff00ff;
  uint64_t e = 0x0000ffff0000ffff;
  uint64_t f = 0x00000000ffffffff;
  
  uint64_t count = i;
  
  count = (count & a) + (count >>1 & a);
  count = (count & b) + (count >> 2 & b);
  count = (count & c) + (count >> 4 & c);
  count = (count & d) + (count >> 8 & d);
  count = (count & e) + (count >> 16 & e);
  count = (count & f) + (count >> 32 & f);
  
  return count;
}
}  // namespace algorithm

#endif  //  __ONE_NUM_H_
