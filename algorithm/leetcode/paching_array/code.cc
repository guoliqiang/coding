// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-27 19:01:09
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int MinPatches(std::vector<int>& nums, int n) {
  int ans = 0;
  long long int sum = 0;
  for (int i = 0; i < nums.size(); i++) {
    if (sum >= n) break;
    while (nums[i] > sum + 1 && sum < n) {
      ans++;
      sum += (sum + 1);
    }
    sum += nums[i];
  }
  while (sum < n) {
    ans++;
    sum += (sum + 1);
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
