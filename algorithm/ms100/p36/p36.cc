// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 10:32:12
// File  : p36.cc
// Brief :
#include "p36.h"

using namespace algorithm;

int main(int argc, char** argv) {
  std::string in = "They are students....";
  std::string el = "aeiou";
  StringEliminate(&in, el);
  LOG(INFO) << in;
  return 0;
}
