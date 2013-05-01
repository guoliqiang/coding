// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-04-13 00:59:01
// File  : debug_util_unittest.cc
// Brief :
#include "../public/debug_util.h"
#include <iostream>
int f() {
  StackTrace trace;
  trace.PrintBacktrace();
}
int main(int argc, char** argv) {
  int a = 10;
  a++;
  // delete &a;
  // std::cout << DebugUtil::BeingDebugged() << std::endl;
  // DebugUtil::BreakDebugger();
  // StackTrace trace;
  // trace.PrintBacktrace();
  f();
  std::cout << "end" << std::endl;
  return 0;
}
