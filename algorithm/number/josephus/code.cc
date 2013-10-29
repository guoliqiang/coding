// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-29 05:27:48
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

int LastNum(int n, int m) {
  if (n == 0) return 0;
  else {
    return (LastNum(n - 1, m) + m) % n;
  }
}

int Last(int n, int m) {
  return LastNum(n, m) + 1;
}

int LastNum2(int n, int m) {
  int rs = 0;
  for (int i = 1; i <= n; i++) {
    rs = (rs + m) % i;
  }
  return rs;
}

int Last2(int n, int m) {
  return LastNum2(n, m) + 1;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Last(4, 2) << " " << Last2(4, 2);
  return 0;
}
