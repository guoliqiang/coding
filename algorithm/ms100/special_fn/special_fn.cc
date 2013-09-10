// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 20:05:04
// File  : special_fn.cc
// Brief :

#include "./special_fn.h"
#include "base/public/logging.h"

using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  LOG(INFO) << fn(10) << " " << fn2(10);
  return 0;
}
