// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 21:31:42
// File  : leftrotate.cc
// Brief :
#include "./leftrotate.h"

using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  std::string str = "abcdef";
  leftrotate(str, 2);
  LOG(INFO) << str;
  str = "abcdefg";
  leftrotate_v2(str, 3);
  LOG(INFO) << str;
  return 0;
}
