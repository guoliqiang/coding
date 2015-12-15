// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-15 20:58:43
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

int MaximumGap(std::vector<int> nums) {
  int n = nums.size();
  if (n < 2) return 0;

  int min = nums[0];
  int max = nums[0];
  for (int i = 0; i < n; i++) {
    min = std::min(min, nums[i]);
    max = std::max(max, nums[i]);
  }
  if (max == min) return 0;

  double interval = (double)(max - min) / (n - 1);
  std::vector<std::pair<int, int> > bucket(n, std::pair<int, int>(0, 0));
  std::vector<int> size(n, 0);

  for (int i = 0; i < n; i++) {
    int index = (nums[i] - min) / interval;
    if (size[index] == 0) {
      bucket[index] = std::make_pair(nums[i], nums[i]);
    } else {
      bucket[index].first = std::min(bucket[index].first, nums[i]);
      bucket[index].second = std::max(bucket[index].second, nums[i]);
    }
    size[index]++;
  }
  int before = 0;
  int rs = 0;
  for (int i = 1; i < n; i++) {
    if (size[i] > 0) {
      rs = std::max(rs, bucket[i].first - bucket[before].second);
      before = i;
    }
  }
  return rs;
}

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(1);
  vec.push_back(1);
  vec.push_back(1);
  vec.push_back(1);
  vec.push_back(1);
  LOG(INFO) << MaximumGap(vec);
  return 0;
}
