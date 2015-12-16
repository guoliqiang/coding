// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 22:17:57
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

void Rotate(std::vector<int>& nums, int k) {
  if (nums.size() == 0 || k % nums.size() == 0) return;
  k %= nums.size();
  std::reverse(nums.begin(), nums.begin() + nums.size() - k);
  std::reverse(nums.begin() + nums.size() - k, nums.end());
  std::reverse(nums.begin(), nums.end());
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> vec;
  for (int i = 1; i <= 7; i++) vec.push_back(i);
  Rotate(vec, 3);
  LOG(INFO) << JoinVector(vec);
  return 0;
}
