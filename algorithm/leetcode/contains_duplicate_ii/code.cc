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

namespace NB {
bool containsNearbyDuplicate(std::vector<int>& nums, int k) {
   std::unordered_set<int> s;

   if (k <= 0) return false;
   if (k >= nums.size()) k = nums.size() - 1;

   for (int i = 0; i < nums.size(); i++)
   {
       if (i > k) s.erase(nums[i - k - 1]);
       if (s.find(nums[i]) != s.end()) return true;
       s.insert(nums[i]);
   }

   return false;
}
}  // namespcace NB

int main(int argc, char** argv) {
  return 0;
}
