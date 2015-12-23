// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 21:44:44
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int LengthOfLIS(const std::vector<int>& nums) {
  std::vector<int> tmp;
  for (int i = 0; i < nums.size(); i++) {
    int b = 0;
    int e = tmp.size() - 1;
    while (b <= e) {
      int mid = b + (e - b) / 2;
      if (tmp[mid] < nums[i]) {
        b = mid + 1;
      } else {
        e = mid - 1;
      }
    }
    if (b == tmp.size()) tmp.push_back(nums[i]);
    else tmp[b] = nums[i];
  }
  return tmp.size();
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
