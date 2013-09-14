// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 20:34:08
// File  : pick_diff_two.cc
// Brief :
#include "pick_diff_two.h"
#include "base/public/logging.h"

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(10);
  foo.push_back(15);
  foo.push_back(1);
  foo.push_back(12);
  foo.push_back(12);
  foo.push_back(10);
  int d1 = 0;
  int d2 = 0;
  PickDiffTwo(foo, &d1, &d2);
  LOG(INFO) << d1 << " " << d2;
  return 0;
}
