// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 12:33:54
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int MissingNumber(std::vector<int>& nums) {
  int sum  = 0;
  for (int i = 0; i < nums.size(); i++) {
    sum += (i + 1);
    sum -= nums[i];
  }
  return sum;
}
}  // namespace alogrithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
