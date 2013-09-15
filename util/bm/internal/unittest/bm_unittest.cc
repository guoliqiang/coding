// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 18:08:45
// File  : bm_unittest.cc
// Brief :
#include "../../public/bm.h"
#include "base/public/logging.h"

using namespace util;

int main(int argc, char** argv) {
  std::string pattern = "aab";
  std::string str = "abababaabab";
  BM<std::string, char> bm(pattern);
  LOG(INFO) << bm.Match(str);
  return 0;
}
