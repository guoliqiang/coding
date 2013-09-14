// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-13 22:27:26
// File  : pow.cc
// Brief :

#include "pow.h"
#include "base/public/logging.h"
using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << "pow(5, 3):" << pow(5, 3);
  LOG(INFO) << "pow(4, 3):" << pow(4, 3);
  LOG(INFO) << "pow(2.5, 3):" << pow(2.5, 3);
  LOG(INFO) << "pow(0, 3):" << pow(0, 3);

  return 0;
}
