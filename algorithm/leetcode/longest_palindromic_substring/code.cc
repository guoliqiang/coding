// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-20 21:29:30
// File  : code.cc
// Brief :
#include "code.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "abcbabb";
  LOG(INFO) << LongestPalindrome(str);
  return 0;
}
