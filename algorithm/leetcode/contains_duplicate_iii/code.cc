// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 21:44:17
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool ContainsNearbyAlmostDuplicate(std::vector<int>& nums, int k, int t) {
  int n = nums.size();
  std::multimap<int, int> m;
  for (int i = 0; i < n; i++) {
    m.insert(std::make_pair(nums[i], i));
  }
  for (int i = 0; i < n; i++) {
    std::multimap<int, int>::iterator it = m.lower_bound(nums[i] - t);
    while(it != m.end() && it->second != i && fabs(it->first - nums[i]) <= t) {
      if (it->second != i && fabs(it->second - i) <= k) return true;
      it++;
    }
  }
  return false;
}
} // namespace algorithm

int main(int argc, char** argv) {
  return 0;
}


