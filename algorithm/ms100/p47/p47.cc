// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-13 23:09:50
// File  : p47.cc
// Brief :

#include "p47.h"
#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "stdlib.h"
#include <algorithm>

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 0; i < 30; i++) {
    foo.push_back(100);
  }
  for (int i = 0; i < 29; i++) {
    foo.push_back(random() % 200);
  }
  for (int i = 0; i < foo.size(); i++) {
    std::swap(foo[i], foo[random() % foo.size()]);
  }
  LOG(INFO) << JoinVector(foo);
  LOG(INFO) << MoreHalfNum(foo);
  return 0;
}
