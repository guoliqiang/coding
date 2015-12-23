// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 14:50:18
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int HIndex(std::vector<int>& citations) {
  int cnt = 1;
  int max = 0;
  for (int i = citations.size() - 1; i >= 0; i--) {
    if (citations[i] >= cnt) max = cnt;
    cnt++;
  }
  return max;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
