// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 11:38:39
// File  : young_matrix.h
// Brief :
// 杨式矩阵等价于根节点是右上的二元查找树

#ifndef  __YOUNG_MATRIX_H_
#define  __YOUNG_MATRIX_H_

#include "stdlib.h"
#include <algorithm>
#include <iostream>
#include "base/public/logging.h"

namespace algorithm {

template <typename type>
void BuildYoung(std::vector<type> & v, int n,
                std::vector<std::vector<type> > * young) {
  CHECK(v.size() == n * n);
  sort(v.begin(), v.end());
  for (int i = 0; i < n; i++) {
    std::vector<type> foo;
    foo.resize(n);
    young->push_back(foo);
  }
  int index = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
      (*young)[i][j] = v[index++];
      if (i != j) (*young)[j][i] = v[index++];
    }
  }
}

template <typename type>
void OutYoung(std::vector<std::vector<type> > & young) {
  for (int i = 0; i < young.size(); i++) {
    for (int j = 0; j < young.size(); j++) {
      std::cout << young[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

template <typename type>
bool YoungFind(std::vector<std::vector<type> > & young,
               int f) {
  int i = 0;
  int j = young.size() - 1;
  while (young[i][j] != f) {
    if (i < 0 || j < 0) return false;
    if (young[i][j] < f) j--;
    if (young[i][j] > f) i--;
    return true;
  }
  return false;
}


}  // namespace algorithm
#endif  // __YOUNG_MATRIX_H_
