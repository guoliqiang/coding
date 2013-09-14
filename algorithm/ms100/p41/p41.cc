// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 20:02:01
// File  : p41.cc
// Brief :
#include "p41.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(3);
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(64);
  foo.push_back(87);
  foo.push_back(4);
  foo.push_back(41);
  OutMin(foo);
  return 0;
}
