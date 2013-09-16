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
  SetMatrix(matrix, 0, 1);
  SetMatrix(matrix, 0, 3);
  SetMatrix(matrix, 2, 3);
  SetMatrix(matrix, 2, 5);
  SetMatrix(matrix, 1, 4);
  std::vector<int> left;
  left.push_back(0);
  left.push_back(2);
  left.push_back(4);
  Hungarian(matrix, &max_matching, left);\
  OutMatrix(max_matching);
  
  std::vector<int> link;
  Hungarian(matrix, left, &link);
  LOG(INFO) << JoinVector(link);

  return 0;
}
