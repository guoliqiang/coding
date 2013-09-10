// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 16:15:06
// File  : max_sub_array.cc
// Brief :

#include "./max_sub_array.h"
#include <stdlib.h>
#include "base/public/string_util.h"

using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 0; i < 10; i++) {
    foo.push_back(random() % 100  - 50);
  }
  int begin = 0;
  int len = 0;
  LOG(INFO) << MaxSubArray(foo, &begin, &len);
  LOG(INFO) << "range:" << begin << "~" << begin + len - 1;
  LOG(INFO) << JoinVector(foo);
  return 0;
}

