// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 23:02:00
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::vector<int> ProductExceptSelf(std::vector<int>& nums) {
  int n = nums.size();
  std::vector<int> ans(n, 0);
  for (int i = n - 1; i >= 0; i--) {
    if (i == n - 1) ans[i] = nums[i];
    else ans[i] = ans[i + 1] * nums[i];
  }
  int cur = 1;
  for (int i = 0; i < n; i++) {
    if (i == 0) ans[i] = ans[i + 1];
    else if (i == n - 1) ans[i] = cur;
    else {
      ans[i] = cur * ans[i + 1];
    }
    cur *= nums[i];
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);

  LOG(INFO) << JoinVector(ProductExceptSelf(vec));
  return 0;
}
