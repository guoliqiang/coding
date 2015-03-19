// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-07-09 12:44:42
// File  : time_unittest.cc
// Brief :
#include "../public/time.h"
#include "../public/logging.h"
#include <iostream>

using namespace base;

int main(int argc, char** argv) {
  LOG(INFO) << "\\\\";
  int64_t t = -1;
  std::cout << std::hex << t << std::endl;
  double b= GetTimeInUsec();
  int rs = 15777777777.00/834.00;
  rs = rs +1;
  double e = GetTimeInUsec();
  LOG(INFO) << b << "~" << e << " " << e -b << " micro seconds ";
  return 0;
}
