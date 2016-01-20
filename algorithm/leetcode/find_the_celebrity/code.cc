// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 22:23:06
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool knows(int a, int b) {
  return true;
}

int findCelebrity(int n) {
  int candi = 0;
  for (int i = 1; i < n; i++) {
    if (knows(i, candi) == false) {
      candi = i;
    }
  }
  for (int i = 0; i < n; i++) {
    if (i != candi && (!knows(i, candi) || knows(candi, i))) return -1;
  }
  return candi;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}

