// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 15:56:03
// File  : next_permutaion.cc
// Brief :
#include "next_permutation.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::string t = "452376002";
  while (NextPermutation(t)) {
    LOG(INFO) << t;
  }
  HERE(INFO);
  t = "452376002";
  while (PrePermutation(t)) {
    LOG(INFO) << t;
  }
 
  return 0;
}
