// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 20:01:49
// File  : special_fn.h
// Brief :

#ifndef  __SPECIAL_FN_H_
#define  __SPECIAL_FN_H_
namespace algorithm {
// wrong can not use ?
int fn(int n) {
  return n ? n + fn(n-1) : 0;
}

int fn2(int n) {
  return ((n + 1) * n) / 2;
}

// 1 new a[n]
// 2.!!n
// 3 template
template <int N>
int fn3() {
  return N + fn3<N-1>();
}
template <>
int fn3<1>() {
  return 1;
}

}  // namespace algorithm

#endif  //__SPECIAL_FN_H_
