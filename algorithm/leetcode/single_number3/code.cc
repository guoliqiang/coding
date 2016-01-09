// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 00:47:36
// File  : code.cc
// Brief :

std::vector<int> SingleNumber(std::vector<int>& nums) {
  int n = nums.size();
  int t = 0;
  for (int i = 0; i < n; i++) t ^= nums[i];
  int flag = (t & (t - 1)) ^ t;

  int ans1 = 0;
  int ans2 = 0;
  for (int i = 0; i < n; i++) {
    if (nums[i] & flag) ans1 ^= nums[i];
    else ans2 ^= nums[i];
  }

  std::vector<int> ans;
  ans.push_back(ans1);
  ans.push_back(ans2);
  return ans;
}
