// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-11-30 18:13:52
// File  : code.cc
// Brief :


#include "base/public/common_ojhead.h"

namespace algorithm {
// n !/ (m! * (n - m)!)
// 这种code会很容易越界

int Com(int  n, int m) {
  if (m > n) return 0;   // bug fixed
  m = std::min(m, n - m);
  int rs = 1;
  int j = 2;
  for (int i = 0; i < m; i++) {
    rs *= (n - i);
    while (j <= m && rs % j == 0) {
      rs /= j;
      j++;
    }
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
