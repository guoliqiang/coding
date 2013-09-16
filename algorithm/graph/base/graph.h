// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-16 11:17:46
// File  : graph.h
// Brief :

#ifndef  __BASE_H_
#define  __BASE_H_

#include <iostream>
#include "base/public/logging.h"

namespace algorithm {
// adjacency matrix
void AdjaMatrix(std::vector<std::vector<int> > & matrix, int size) {
  for (int i = 0; i < size; i++) {
    matrix.push_back(std::vector<int>(size, 0));
  }
}

void SetMatrix(std::vector<std::vector<int> > & matrix, int i, int j, int v = 1) {
  CHECK(i < matrix.size());
  CHECK(j < matrix.size());
  matrix[i][j] = v;
  matrix[j][i] = v;
}

void OutMatrix(std::vector<std::vector<int> > & matrix) {
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix.size(); j++) {
       std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

}  // namespace algorithm


#endif  //  __BASE_H_
