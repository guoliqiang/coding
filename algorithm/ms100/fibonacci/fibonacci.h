// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 17:18:17
// File  : fibonacci.h
// Brief :

#ifndef  __FIBONACCI_H_
#define  __FIBONACCI_H_

#include <vector>
#include "base/public/logging.h"

namespace algorithm {
int fibonacci(int n) {
  if (n == 1) return 1;
  if (n == 2) return 1;
  return fibonacci(n - 1) + fibonacci(n - 2);
}

int fibonacci_dp(int n) {
  std::vector<int> foo;
  foo.resize(n);
  foo[0] = 1;
  foo[1] = 1;
  for (int i = 2; i < foo.size(); i++) {
    foo[i] = foo[i - 1] + foo[i - 2];
  }
  return foo.back();
}

std::vector<int> matrix(const std::vector<int> & a,
                        const std::vector<int> & b) {
  std::vector<int> rs;
  rs.resize(a.size());
  rs[0] = a[0] * b[0] + a[1] * b[2];
  rs[1] = a[0] * b[1] + a[1] * b[3];
  rs[2] = a[2] * b[0] + a[3] * b[2];
  rs[3] = a[2] * b[1] + a[3] * b[3];
  return rs;
}

int fibonacci_matrix(int n) {
  if (n == 1 || n == 2) return 1;
  n--;
  std::vector<int> rs;
  std::vector<int> foo;
  rs.resize(4);
  foo.resize(4);
  foo[0] = 1; foo[1] = 1; foo[2] = 1; foo[3] = 0;
  rs[0] = 1; rs[1] = 0; rs[2] = 0; rs[3] = 1;
  // Note rs == e(rs[1] and rs[2] = 0),
  // this bug need much time to fix, ai....
  while (n) {
    if (n & 0x1) {
      rs = matrix(rs, foo);
      // LOG(INFO) << rs[0] << " at: " << n;
    }
    foo = matrix(foo, foo);
    // LOG(INFO) << foo[0] << " at: " << n;
    n = n >> 1;
  }
  return rs[0];
}

}  // namespace algorithm

#endif  //  __FIBONACCI_H_
