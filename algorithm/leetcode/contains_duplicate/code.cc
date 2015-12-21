// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 21:10:10
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool ContainsDuplicate(std::vector<int>& nums) {
  std::set<int> set;
  for (int i = 0; i < nums.size(); i++) {
    if (set.count(nums[i])) return true;
    set.insert(nums[i]);
  }
  return false;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
