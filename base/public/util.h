// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 02:44:20
// File  : util.h
// Brief :

#ifndef  __UTIL_H_
#define  __UTIL_H_

namespace base {
template <class T>
inline T max(const T & a, const T & b) {
  return (a > b) ? a : b;
}

template <class T>
inline T min(const T & a, const T & b) {
  return (a < b) ? a : b;
}

template <class T>
inline void swap(T & a, T & b) {
  if (&a == &b) return;  // bug fix
  a ^= b;
  b ^= a;
  a ^= b;
}

};
#endif  //  __UTIL_H_
