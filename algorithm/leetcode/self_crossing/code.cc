// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-03-20 23:15:02
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
bool isSelfCrossing(std::vector<int> & x) {
  for (int i = 3; i < x.size(); i++) {
    if (x[i] >= x[i - 2] && x[i - 1] <= x[i - 3]) {
      // Case 1: current line crosses the line 3 steps ahead of it
      return true;
    } else if (i >= 4 && x[i - 1] == x[i - 3] && x[i] + x[i - 4] >= x[i - 2]) {
      // Case 2: current line crosses the line 4 steps ahead of it
      return true;
    } else if (i >= 5 && x[i - 2] >= x[i - 4] &&
               x[i] + x[i - 4] >= x[i - 2] &&
               x[i - 1] <= x[i - 3] &&
               x[i - 1] + x[i - 5] >= x[i-3]) {
      // Case 3: current line crosses the line 6 steps ahead of it
      return true;
    }
  }
  return false;
}
}  // namespace algorithm


int main(int argc, char** argv) {
  return 0;
}
