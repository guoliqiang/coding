// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 16:40:16
// File  : p29.cc
// Brief :
#include "p29.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 0; i < 10; i++) {
    foo.push_back(random() % 10);
  }
  SwapOddEven(foo);
  LOG(INFO) << JoinVector(foo);
  return 0;
}
