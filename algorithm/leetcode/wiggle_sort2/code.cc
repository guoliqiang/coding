// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 21:52:34
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
// M S S S
//  L M M
void WiggleSort(std::vector<int> & nums) {
  int n = nums.size();
  if (n == 0) return;
  std::nth_element(nums.begin(), nums.begin() + n / 2, nums.end());
  int mid = nums[n / 2];

  int idx1 = -1;
  int idx2 = n;
  int j = 0;
  while (j < idx2) {
    if (nums[j] < mid) {
      std::swap(nums[j++], nums[++idx1]);
    } else if (nums[j] > mid) {
      std::swap(nums[j], nums[--idx2]);
    } else {
      j++;
    }
  }
  std::vector<int> ans;
  idx1 = n - 1;
  idx2 = n % 2 ? n / 2 : n / 2 - 1;
  for (int i = 0; i < n / 2; i++) {
    ans.push_back(nums[idx2--]);
    ans.push_back(nums[idx1--]);
  }
  if (n % 2) ans.push_back(nums[idx2--]);
  nums = ans;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(4);
  vec.push_back(5);
  vec.push_back(5);
  vec.push_back(6);
  LOG(INFO) << JoinVector(vec);
  WiggleSort(vec);
  LOG(INFO) << JoinVector(vec);
  return 0;
}
