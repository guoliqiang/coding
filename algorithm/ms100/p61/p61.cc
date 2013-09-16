// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-16 23:20:36
// File  : p61.cc
// Brief :
#include "p61.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(2);
  foo.push_back(4);
  foo.push_back(1);
  foo.push_back(16);
  foo.push_back(7);
  foo.push_back(5);
  foo.push_back(11);
  foo.push_back(9);
  LOG(INFO) << MaxDiff(foo) << " " << MaxDiff2(foo);
  return 0;
}
