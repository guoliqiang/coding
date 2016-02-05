// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 19:32:47
// File  : code.cc
// Brief :

// https://leetcode.com/discuss/36966/solution-explained

/*
Top K int in a large stream
O(N)解法: 2K windows, Quick Select, throw K away.
http://www.1point3acres.com/bbs/forum.php?mod=viewthread&tid=148682&extra=page%3D2%26filter%3Dsortid%26sortid%3D311%26searchoption%5B3046%5D%5Bvalue%5D%3D1%26searchoption%5B3046%5D%5Btype%5D%3Dradio%26sortid%3D311

有N/K 个batches， 所以是O(K * N / K) = O(n)
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
// best O(n), worest O(n^2), if want to O(n), need to shuffle the nums first
void Shuffle(std::vector<int> & nums) {
  for (int i = 0; i < nums.size(); i++) {
    int idx = random() % (nums.size() - i);
    std::swap(nums[i], nums[i + idx]);
  }
}

int FindKthLargest(std::vector<int> nums, int k) {
  // Shuffle(nums);
  int begin = 0;
  int end = nums.size() - 1;

  while(begin <= end) {
    int idx = begin - 1;
    for (int i = begin; i < end; i++) {
      if (nums[i] > nums[end]) {
        std::swap(nums[++idx], nums[i]);
      }
    }
    std::swap(nums[++idx], nums[end]);
    if (idx - begin + 1 == k) {
      return nums[idx];
    } else if (idx - begin + 1 > k) {
      end = idx - 1;
    } else {
      k = k - (idx - begin + 1);
      begin = idx + 1;
    }
  }
  return 0;
}

}  // namespace algorithm

namespace NB {
int FindKthLargest(std::vector<int> nums, int k) {
        int n = nums.size();
        std::priority_queue<int, std::vector<int>, std::greater<int> > queue;
        for (int i = 0; i < n; i++) {
            queue.push(nums[i]);
            if (queue.size() > k) queue.pop();
        }
        return queue.top();
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(-1);
  vec.push_back(-1);
  LOG(INFO) << FindKthLargest(vec, 2);
  return 0;
}
