// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 18:08:45
// File  : kmp_unittest.cc
// Brief :
#include "../../public/kmp.h"
#include "base/public/logging.h"

using namespace util;

int main(int argc, char** argv) {
  std::string pattern = "aab";
  std::string str = "abababaabab";
  BM<std::string, char> kmp(pattern);
  LOG(INFO) << kmp.Match(str);
  return 0;
}
