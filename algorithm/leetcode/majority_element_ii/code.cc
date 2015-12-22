// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 21:25:57
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::vector<int> MajorityElement(std::vector<int>& nums) {
  int num1 = 0;
  int cnt1 = 0;
  int num2 = 0;
  int cnt2 = 0;
  for (int i = 0; i < nums.size(); i++) {
    if (cnt1 == 0 || nums[i] == num1) {
      cnt1++;
      num1 = nums[i];
    } else if (cnt2 == 0 || nums[i] == num2) {
      cnt2++;
      num2 = nums[i];
    } else {
      cnt1--;
      cnt2--;;
    }
  }

  cnt1 = 0;
  cnt2 = 0;
  for (int i = 0; i < nums.size(); i++) {
    if (nums[i] == num1) cnt1++;
    if (nums[i] == num2) cnt2++;
  }
  std::vector<int> ans;
  if (cnt1 > nums.size() / 3) ans.push_back(num1);
  if (cnt2 > nums.size() / 3 && num2 != num1) ans.push_back(num2);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
