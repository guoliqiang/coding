// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 11:21:53
// File  : code.cc
// Brief :

// 可以扩展twosum问题
#include "base/public/common_ojhead.h"

namespace algorithm {

int ThreeSumSmaller(std::vector<int> & nums, int target) {
  int n = nums.size();
  if (n == 0) return 0;

  int ans = 0;
  std::sort(nums.begin(), nums.end());
  for (int i = 0; i < n; i++) {
    int b = i + 1;
    int e = n - 1;
    while (b < e) {
      if (nums[i] + nums[b] + nums[e] < target) {
        ans += e - b;
        b++;
      } else {
        e--;
      }
    }
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  LOG(INFO) << ThreeSumSmaller(vec, 6);
  return 0;
}
