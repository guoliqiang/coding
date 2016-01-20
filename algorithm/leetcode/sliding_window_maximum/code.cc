// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 18:34:47
// File  : code.cc
// Brief :


#include "base/public/common_ojhead.h"

namespace algorithm {
using namespace std;

vector<int> maxSlidingWindow(vector<int>& nums, int k) {
  std::vector<int> ans;
  std::deque<int> queue;
  for (int i = 0; i < nums.size(); i++) {
    while (queue.size() && queue.back() < nums[i]) queue.pop_back();
    queue.push_back(nums[i]);
    if (i >= k) {
      int idx = i - k;
      if (queue.front() == nums[idx]) queue.pop_front();
    }
    if (i >= k - 1) ans.push_back(queue.front());
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
