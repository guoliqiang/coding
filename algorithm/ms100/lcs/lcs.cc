// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 19:45:09
// File  : lcs.cc
// Brief :

#include "./lcs.h"
using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  std::string a = "BDCABA";
  std::string b = "ABCBDAB";
  LOG(INFO) << lcs(a, b);
  lcs_print(a, b);
  return 0;
}
