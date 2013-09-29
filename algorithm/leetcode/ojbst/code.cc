// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 10:33:37
// File  : code.cc
// Brief :

#include "ojbst.h"

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str = "0,1";
  InOrder(Make(str));
  PreOrder(Make(str));
  return 0;
}
