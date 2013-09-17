// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-18 00:27:58
// File  : unittest.cc
// Brief :
#include "../public/stl_util-inl.h"
using namespace gtl;

int main(int argc, char** argv) {
  int a = 10;
  int b = 11;
  int c = a + b;
  c++;
  std::vector<int> v1;
  std::vector<int> v2;
  v2 = v2 + v1;
  v2 += v1;
  return 0;
}
