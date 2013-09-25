// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-16 13:16:24
// File  : max_matching.cc
// Brief :
#include "max_matching.h"
#include "base/public/util.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix;
  std::vector<std::vector<int> > max_matching;
  AdjaMatrix(matrix, 6);
  AdjaMatrix(max_matching, 6);
  SetMatrixDouble(matrix, 0, 1);
  SetMatrixDouble(matrix, 0, 3);
  SetMatrixDouble(matrix, 2, 3);
  SetMatrixDouble(matrix, 2, 5);
  SetMatrixDouble(matrix, 1, 4);
  std::vector<int> left;
  left.push_back(0);
  left.push_back(2);
  left.push_back(4);
  Hungarian(matrix, &max_matching, left);\
  OutMatrix(max_matching);
  
  std::vector<int> link;
  Hungarian(matrix, left, &link);
  LOG(INFO) << JoinVector(link);
  
  std::vector<int> min_set;
  FindMinCoverSet(matrix, left, &min_set);
  LOG(INFO) << "min_set:" << JoinVector(min_set);

  link.clear();
  matrix.clear();
  AdjaMatrix(matrix, 3);
  SetMatrix(matrix, 0, 0, 0);
  SetMatrix(matrix, 0, 1, 2);
  SetMatrix(matrix, 0, 2, 0);

  SetMatrix(matrix, 1, 0, 0);
  SetMatrix(matrix, 1, 1, 0);
  SetMatrix(matrix, 1, 2, 3);
  
  SetMatrix(matrix, 2, 0, 0);
  SetMatrix(matrix, 2, 1, 0);
  SetMatrix(matrix, 2, 2, 3);
  /*
  AdjaMatrix(matrix, 5);
  SetMatrix(matrix, 0, 0, 3);
  SetMatrix(matrix, 0, 1, 5);
  SetMatrix(matrix, 0, 2, 5);
  SetMatrix(matrix, 0, 3, 4);
  SetMatrix(matrix, 0, 4, 1);

  SetMatrix(matrix, 1, 0, 2);
  SetMatrix(matrix, 1, 1, 2);
  SetMatrix(matrix, 1, 2, 0);
  SetMatrix(matrix, 1, 3, 2);
  SetMatrix(matrix, 1, 4, 2);
  
  SetMatrix(matrix, 2, 0, 2);
  SetMatrix(matrix, 2, 1, 4);
  SetMatrix(matrix, 2, 2, 4);
  SetMatrix(matrix, 2, 3, 1);
  SetMatrix(matrix, 2, 4, 0);
  
  SetMatrix(matrix, 3, 0, 0);
  SetMatrix(matrix, 3, 1, 1);
  SetMatrix(matrix, 3, 2, 1);
  SetMatrix(matrix, 3, 3, 0);
  SetMatrix(matrix, 3, 4, 0);
  
  SetMatrix(matrix, 4, 0, 1);
  SetMatrix(matrix, 4, 1, 2);
  SetMatrix(matrix, 4, 2, 1);
  SetMatrix(matrix, 4, 3, 3);
  SetMatrix(matrix, 4, 4, 3);
  */
  KM(matrix, &link);
  LOG(INFO) << JoinVector(link);
  return 0;
}
