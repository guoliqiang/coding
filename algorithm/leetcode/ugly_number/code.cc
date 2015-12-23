// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 11:33:46
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool IsUgly(int num) {
    while (num % 2 == 0 && num != 0) num /= 2;
    while (num % 3 == 0 && num != 0) num /= 3;
    while (num % 5 == 0 && num != 0) num /= 5;
    return num == 1;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
