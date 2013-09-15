// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 11:37:28
// File  : add.cc
// Brief :
#include "add.h"
#include "base/public/logging.h"
using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << AddWithoutArithmetic(5, 7);
  return 0;
}
