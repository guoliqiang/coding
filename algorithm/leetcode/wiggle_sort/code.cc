// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 21:52:34
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void WiggleSort(std::vector<int> & nums) {
  for (int i = 1; i < nums.size(); i++) {
    if (i % 2) {
      if (nums[i] < nums[i - 1]) std::swap(nums[i], nums[i - 1]);
    } else {
      if (nums[i] > nums[i - 1]) std::swap(nums[i], nums[i - 1]);
    }
  }
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(3);
  vec.push_back(5);
  vec.push_back(2);
  vec.push_back(1);
  vec.push_back(6);
  vec.push_back(4);
  LOG(INFO) << JoinVector(vec);
  WiggleSort(vec);
  LOG(INFO) << JoinVector(vec);
  return 0;
}
