// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-20 23:39:24
// File  : code.cc
// Brief :
#include "code.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "PAYPALISHIRING";
  LOG(INFO) << 1 << " " << zigzag(str, 1);
  LOG(INFO) << 2 << " " << zigzag(str, 2);
  LOG(INFO) << 3 << " " << zigzag(str, 3);
  LOG(INFO) << 4 << " " << zigzag(str, 4);
  LOG(INFO) << 5 << " " << zigzag(str, 5);
  LOG(INFO) << 6 << " " << zigzag(str, 6);
  LOG(INFO) << 7 << " " << zigzag(str, 7);
  LOG(INFO) << 8 << " " << zigzag(str, 8);
  return 0;
}
