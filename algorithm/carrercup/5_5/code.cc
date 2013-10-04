// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-04 17:46:15
// File  : code.cc
// Brief :

/*
 * 求解１的个数还有一个更快的方法
 * 见软连接中的文件
 * */

#include "base/public/common_head.h"

namespace algorithm {

int Ones(int a) {
  int num = 0;
  while (a) {
    num++;
    a &= (a - 1);
  }
  return num;
}

int Convert(int a, int b) {
  return Ones(a ^ b);
}

}  // namespace algoruthm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Convert(31, 14);
  return 0;
}
