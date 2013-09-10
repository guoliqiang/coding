// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 17:38:50
// File  : fibonacci.cc
// Brief :

#include "./fibonacci.h"
using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  LOG(INFO) << fibonacci(13);
  LOG(INFO) << fibonacci_dp(13);
  LOG(INFO) << fibonacci_matrix(13);
  return 0;
}
