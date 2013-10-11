// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-11 21:58:59
// File  : sunday_unittest.cc
// Brief :

#include "../../public/sunday.h"
using namespace util;


int main(int argc, char** argv) {
  Sunday<std::string, char> s("search");
  LOG(INFO) << s.Match("substring searhing algorithm");
  return 0;
}
