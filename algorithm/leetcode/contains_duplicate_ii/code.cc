// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 21:22:39
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

bool ContainsNearbyDuplicate(std::vector<int>& nums, int k) {
  std::map<int, int> m;
  for (int i = 0; i < nums.size(); i++) {
    if (m.count(nums[i])) {
      if (i - m[nums[i]] <= k) return true;
    }
    m[nums[i]] = i;
  }
  return false;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
