// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-13 22:22:05
// File  : pow.h
// Brief :

#ifndef  __POW_H_
#define  __POW_H_
namespace algorithm {
double pow (double base, int exp) {
  double rs = 1;
  double tmp = base;
  while (exp) {
    if (exp & 1) rs *= tmp;
    tmp *= tmp;
    exp = exp >> 1;
  }
  return rs;
}

}  // namespace algorithm

#endif  // __POW_H_
