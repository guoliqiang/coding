// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 19:50:05
// File  : swap_string.cc
// Brief :
#include "./swap_string.h"
#include "base/public/logging.h"

using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  std::string foo = "i am a student.";
  LOG(INFO) << foo;
  SwapString(foo);
  LOG(INFO) << foo;
  foo = "  i am a student.";
  LOG(INFO) << foo;
  SwapString(foo);
  LOG(INFO) << foo;
  foo = "i am a student.  ";
  LOG(INFO) << foo;
  SwapString(foo);
  LOG(INFO) << foo;
  foo = "  i am a   student.  ";
  LOG(INFO) << foo;
  SwapString(foo);
  LOG(INFO) << foo;
  return 0;
}
