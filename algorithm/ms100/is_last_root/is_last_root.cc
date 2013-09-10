// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 18:00:12
// File  : is_last_root.cc
// Brief :

#include "./is_last_root.h"
#include "base/public/logging.h"

using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(5);
  foo.push_back(7);
  foo.push_back(6);
  foo.push_back(9);
  foo.push_back(11);
  foo.push_back(10);
  foo.push_back(8);
  LOG(INFO) << IsLastRoot(foo);
  foo.clear();

  foo.push_back(7);
  foo.push_back(4);
  foo.push_back(6);
  foo.push_back(5);
  LOG(INFO) << IsLastRoot(foo);
  return 0;
}
