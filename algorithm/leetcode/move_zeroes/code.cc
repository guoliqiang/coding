// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 15:39:51
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void MoveZeroes(std::vector<int>& nums) {
  int idx = -1;
  for (int i = 0; i < nums.size(); i++) {
    if (nums[i] != 0) {
      std::swap(nums[i], nums[++idx]);
    }
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
