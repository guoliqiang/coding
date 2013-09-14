// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 11:11:55
// File  : stack_sequence.cc
// Brief :

#include "./stack_sequence.h"

using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  std::string input = "12345";
  std::string output = "45321";
  LOG(INFO) << is_stack_sequence(input, output);
  
  input = "12345";
  output = "45231";
  LOG(INFO) << is_stack_sequence(input, output);
  return 0;
}
