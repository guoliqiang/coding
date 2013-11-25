// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-07 16:58:44
// File  : code.cc
// Brief :

/*
 * Write a function called my2DAlloc which allocates a two dimensional array. 
 * Minimize the number of calls to malloc and make sure that the memory is accessible by the notation arr[i][j].
 * */

#include "base/public/common_head.h"

namespace algorithm {
int ** My2Dalloc(int row, int col) {
  int size = row * col * sizeof(int) + row * sizeof(int *);
  void * p = malloc(size);
  int ** t = reinterpret_cast<int **>(p);
  int * m = reinterpret_cast<int*>(t + row);
  // int * m = reinterpret_cast<int*>(p) + row;
  // bug fixed sizeof(int *) = 8; sizeof(int) = 4, t++相当于+(sizeof(int*)),p++相当于+(sizeof(int))
  for (int i = 0; i < row; i++) t[i] = m + col * i;
  return t;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  // LOG(INFO) << sizeof(int *) << " " << sizeof(int);
  // int (* ptr)[10] = new int[10][10];
  int m = 2;
  int n = 3;
  int ** p = My2Dalloc(m, n);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      p[i][j] = i;
    }
  }
  free(p);
  return 0;
}
