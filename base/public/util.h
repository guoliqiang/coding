// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 02:44:20
// File  : util.h
// Brief :

#ifndef  __UTIL_H_
#define  __UTIL_H_

#include <string>

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

template<class T>
std::string ToBinary(T a) {
  std::string rs = "";
  int num = 0;
  while (a) {
    if (a & 1) rs.insert(rs.begin(), '1');
    else rs.insert(rs.begin(), '0');
    a >>= 1;
    if ((++num) % 4 == 0) rs.insert(rs.begin(), ' ');
  }
  return rs;
}

};
#endif  //  __UTIL_H_
