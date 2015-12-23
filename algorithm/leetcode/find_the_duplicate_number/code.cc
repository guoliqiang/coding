// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 19:06:10
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"
// http://segmentfault.com/a/1190000003817671

namespace algorithm {

int FindDuplicate(std::vector<int>& nums) {
  int min = 1;
  int max = nums.size() - 1;
  while (min < max) {
    int mid = min + (max - min) / 2;
    int cnt = 0;
    for (int i = 0; i < nums.size(); i++) {
      if (nums[i] <= mid && nums[i] >= min) cnt++;
    }
    if (cnt > mid - min + 1) {
      max = mid;
    } else {
      min = mid + 1;
    }
  }
  return min;
}
}  // namespace algorithm

namespace NB {
int FindDuplicate(std::vector<int>& nums) {
  int slow = 0;
  int fast = 0;
  do {
    slow = nums[slow];
    fast = nums[nums[fast]];
  } while (slow != fast);

  int find = 0;
  while (find != slow) {
    find = nums[find];
    slow = nums[slow];
  }
  return find;
}
}  // namespace NB
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(1);
  vec.push_back(3);
  vec.push_back(2);
  vec.push_back(2);
  LOG(INFO) << FindDuplicate(vec);
  LOG(INFO) << NB::FindDuplicate(vec);
  return 0;
}
