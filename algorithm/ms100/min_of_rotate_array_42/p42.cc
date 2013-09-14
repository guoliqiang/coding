// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 20:23:54
// File  : p42.cc
// Brief :
#include "p42.h"
#include "base/public/logging.h"

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(5);
  foo.push_back(8);
  foo.push_back(19);
  foo.push_back(2);
  foo.push_back(3);
  foo.push_back(4);
  LOG(INFO) << MinOfRotateArray(foo);
  foo.clear();
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(3);
  foo.push_back(3);
  LOG(INFO) << MinOfRotateArray(foo);
  foo.clear();
  foo.push_back(3);
  foo.push_back(5);
  LOG(INFO) << MinOfRotateArray(foo);
  
  foo.clear();
  foo.push_back(3);
  foo.push_back(3);
  foo.push_back(3);
  foo.push_back(3);
  foo.push_back(3);
  foo.push_back(3);
  foo.push_back(5);
  foo.push_back(5);
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(3);
  foo.push_back(3);
  foo.push_back(3);

  LOG(INFO) << MinOfRotateArray(foo);
  return 0;
}
