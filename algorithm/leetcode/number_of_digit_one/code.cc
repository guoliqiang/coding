// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 22:42:18
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int CountDigitOne(int n) {
  int ones = 0;
  for (long m = 1; m <= n; m *= 10) {
    long a = n / m, b = n % m;
    ones += (a + 8) / 10 * m;
    if(a % 10 == 1) ones += b + 1;
  }
  return ones;
}
}  // namespcae algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
