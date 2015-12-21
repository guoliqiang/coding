// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 19:32:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int FindKthLargest(std::vector<int> nums, int k) {
  int begin = 0;
  int end = nums.size();

  while(begin < end) {
    int idx = begin - 1;
    for (int i = begin; i < end - 1; i++) {
      if (nums[i] > nums[end - 1]) {
        std::swap(nums[++idx], nums[i]);
      }
    }
    std::swap(nums[++idx], nums[end - 1]);
    if (idx - begin + 1 == k) {
      return nums[idx];
    } else if (idx - begin + 1 > k) {
      end = idx;
    } else {
      k = k - (idx - begin + 1);
      begin = idx + 1;
    }
  }
  return 0;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  LOG(INFO) << FindKthLargest(vec, 1);
  LOG(INFO) << FindKthLargest(vec, 2);
  LOG(INFO) << FindKthLargest(vec, 3);
  return 0;
}
