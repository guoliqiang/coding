// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-06 17:33:32
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

int MaxSubArrayLen(std::vector<int>& nums, int k) {
  int n = nums.size();
  if (n == 0) return 0;

  std::map<int, int> idx;
  idx[0] = -1;
  int ans = 0;
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += nums[i];
    if (!idx.count(sum)) {
      idx[sum] = i;
    }
    if (idx.count(sum - k)) {
      ans = std::max(ans, i - idx[sum - k]);
    }
  }
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(1);
  vec.push_back(-1);
  vec.push_back(5);
  vec.push_back(-2);
  vec.push_back(3);
  LOG(INFO) << MaxSubArrayLen(vec, 3);
  return 0;
}
