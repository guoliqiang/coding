// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-16 23:32:00
// File  : p62.cc
// Brief :
#include "base/public/logging.h"

bool Fun1(const char * str) {
  LOG(INFO) << str;
  return false;
}

bool Fun2(const char * str) {
  LOG(INFO) << str;
  return true;
}

// left to right
int main(int argc, char ** argv) {
  bool res1, res2;
  res1 = (Fun1("a") &&
          Fun2("b")/*not need to computed*/) ||
          (Fun1("c") || Fun2("d"));
  // acd
  res2 = (Fun1("a") && Fun2("b")/*not need to compute*/) &&
         (Fun1("c") || Fun2("d"))/*not need to compute*/;
  return res1 || res2;
}
