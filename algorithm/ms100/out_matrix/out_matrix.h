// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 10:19:09
// File  : out_matrix.h
// Brief :

#ifndef  __OUT_MATRIX_H_
#define  __OUT_MATRIX_H_

#include <iostream>
#include "base/public/logging.h"

namespace algorithm {
void OutMatrix(int ** ptr, int b, int n) {
  // std::cout << std::endl << "b:" << b << " n:" << n << std::endl;;
  for (int i = b; i < b + n; i++) {
    std::cout << ptr[b][i] << " ";
  }
  for (int i = b + 1; i < b + n; i++) {
    std::cout << ptr[i][b + n - 1] << " ";
  }
  for (int i = b + n - 1 - 1; i >= b; i--) {
    std::cout << ptr[b + n -1][i] << " ";
  }
  for (int i = b + n - 1 - 1; i > b; i--) {
    std::cout << ptr[i][b] << " ";
  }
}

void OutMatrix(int n) {
  int ** ptr = new int *[n];
  for (int i = 0; i < n; i++) {
    ptr[i] = new int[n];
  }
  int foo = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      ptr[i][j] = foo++;
    }
  }
  int t = ((double)n + 0.5)/2.0 + 1;
  // LOG(INFO) << "t:" << t;
  for (int i = 0; i < t; i++) {
    OutMatrix(ptr, i, n);
    n -= 2;
  }
  std::cout << std::endl;
  for (int i = 0; i < n; i++) {
    delete [] ptr[i];
  }
  delete [] ptr;
}

}  // namespace algorithm

#endif  //__OUT_MATRIX_H_
