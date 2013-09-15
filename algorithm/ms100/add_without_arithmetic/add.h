// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 11:37:33
// File  : add.h
// Brief :

#ifndef  __ADD_H_
#define  __ADD_H_
namespace algorithm {
int AddWithoutArithmetic(int a, int b) {
  if (b == 0) return a;
  int rs = a ^ b;
  int carry = (a & b) << 1;
  return AddWithoutArithmetic(rs, carry);
}
}  // namespace algorithm


#endif  //__ADD_H_
