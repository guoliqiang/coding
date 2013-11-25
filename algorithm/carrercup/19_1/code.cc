// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 10:05:59
// File  : code.cc
// Brief :

/*
 * Write a function to swap a number in place without temporary variables.
 * */

#include "base/public/common_head.h"

namespace algorithm {

// 必须有if判断，否则交换同一个变量会出错

template<typename T>
void Swap(T & a, T & b) {
  if (&a == &b) return;  // bug fix
  a ^= b;
  b ^= a;
  a ^= b;
}

template<typename T>
void Swap2(T & a, T & b) {
  if (&a == &b) return;  // bug fix
  b = a - b;
  a = a - b;
  b = a + b;
  /*  or
   *  a = a - b;
   *  b = a + b;
   *  a = b - a;
   *
   * */
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int ia = 10;
  int & ib = ia;
  int & a1 = ia;
  int & a2 = ia;
  LOG(INFO) << &ia << " " << &a1 << " " << &a2;
  
  LOG(INFO) << ia << " " << ib;
  Swap2(ia, ib);
  LOG(INFO) << ia << " " << ib;
  return 0;
}
