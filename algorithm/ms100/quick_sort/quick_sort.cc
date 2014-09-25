// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-16 00:57:58
// File  : quit_sort.cc
// Brief :
#include "quick_sort.h"
#include "base/public/string_util.h"
using namespace algorithm;

int main(int argc, char** argv) {
  srand(time(NULL));
  std::vector<int> foo;
  std::vector<int> bar;
  for (int i = 0; i < 10; i++) {
    foo.push_back(rand() % 100);
    bar.push_back(10);
  }
  Qsort2(foo);
  Qsort2(bar);
  LOG(INFO) << JoinVector(foo);
  LOG(INFO) << JoinVector(bar);
  return 0;
}
