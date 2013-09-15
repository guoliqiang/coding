// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 11:54:15
// File  : big.cc
// Brief :
#include "big.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::string a = "123";
  std::string b = "22";
  LOG(INFO) << Big(a, b);
  a = "1238976543219807";
  b = "229806584321";
  LOG(INFO) << Big(a, b);
  return 0;
}
