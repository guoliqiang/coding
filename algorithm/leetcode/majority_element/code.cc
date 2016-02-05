// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 10:36:41
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int MajorityElement(std::vector<int>& nums) {
  int cnt = 1;
  int cur = nums[0];
  for (int i = 1; i < nums.size(); i++) {
    if (cur == nums[i]) {
      cnt++;
    } else {
      cnt--;
      if (cnt == 0) {
        cur = nums[i];
        cnt++;
      }
    }
  }
  return cur;
}

}  // namespace algorithm

namespace clear {
int majorityElement(std::vector<int>& nums) {
  int cnt = 0;
  int ans = 0;
  for (int i = 0; i < nums.size(); i++) {
      if (cnt == 0 || ans == nums[i]) {
          cnt++;
          ans = nums[i];
      } else {
          cnt--;
      }
  }
  return ans;
}
}  // namespace clear

namespace other {
// 如果是排序好的就可以直接用二分查找找中间元素的左右边界然后判断是不是大于n/2
int MajorityElement(std::vector<int>& nums) {
  nth_element(nums.begin(), nums.begin() + nums.size() / 2, nums.end());
  return nums[nums.size() / 2];
}

}  // namespace other

namespace extend {
// find element counts greater than  n / k
// 当k比较大时，还不如直接用个map统计复杂度底
std::vector<int> Major(std::vector<int> & nums, int k) {
  std::vector<int> v(k - 1, 0);
  std::vector<int> c(k - 1, 0);

  for (int i = 0; i < nums.size(); i++) {
    int j = 0;
    for (j = 0; j < k - 1; j++) {
      if (c[j] == 0 || v[j] == nums[i]) {
        c[j]++;
        v[j] = nums[i];
        break;
      }
    }
    if (j == k) {
      for (int m = 0; m < k - 1; k++) c[m]--;
    }
  }
  for (int i = 0; i < k - 1; i++) c[i] = 0;
  for (int i = 0; i < nums.size(); i++) {
    for (int j = 0; j < k - 1; j++) {
      if (v[j] == nums[i]) c[j]++;
    }
  }
  std::vector<int> ans;
  for (int i = 0; i < k - 1; i++) {
    if (c[i] > nums.size() / k) ans.push_back(v[i]);
  }
  return ans;
}
}  // namespace extend

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
