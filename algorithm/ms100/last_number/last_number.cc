// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 11:56:20
// File  : last_numner.cc
// Brief :

#include "last_number.h"
#include "base/public/logging.h"

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  NumList(10, 3, &vec);
  LOG(INFO) << JoinVector(vec);

  LOG(INFO) << LastNum(3, 2) << " " << LastNum2(3, 2);
  LOG(INFO) << LastNum(5, 5) << " " << LastNum2(5, 5);
  LOG(INFO) << LastNum(10, 3) << " " << LastNum2(10, 3);
  LOG(INFO) << LastNum(30, 7) << " " << LastNum2(30, 7);
  LOG(INFO) << LastNum(35, 21) << " " << LastNum2(35, 21);
  return 0;
}
