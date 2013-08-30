// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-08-14 17:40:58
// File  : flag_unitest.cc
// Brief :

#include "base/public/logging.h"

DEFINE_string(test, "test", "");

int main(int argc, char** argv) {
  base::ParseCommandLineFlags(&argc, &argv, false);
  LOG(INFO) << sizeof(bool);
  base::ReadFromFlagsFile("./flag.txt", NULL, false);
  LOG(INFO) << FLAGS_test;
  return 0;
}
