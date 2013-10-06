// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 17:05:48
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

void Trace(std::string str, int left_num, int right_num, int n) {
  if (left_num == n) {
    for (int i = right_num + 1; i <= n; i++) str += ")";
    LOG(INFO) << str;
  } else {
    Trace(str + "(", left_num + 1, right_num, n);
    if (right_num < left_num) {
      Trace(str + ")", left_num, right_num + 1, n);
    }
  }
}

void Trace(int n) {
  int left_num = 0;
  int right_num = 0;
  std::string str = "";
  Trace(str, left_num, right_num, n);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 2;
  Trace(n);
  return 0;
}
