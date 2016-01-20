// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-10-25 15:59:40
// File  : code.cc
// Brief :

#include <vector>
#include "base/public/common_ojhead.h"

using namespace std;

namespace algorithm {

// 188 / 188 test cases passed.
// Status: Accepted
// Runtime: 48 ms
// Submitted: 0 minutes ago
int FindMin(const std::vector<int> & vec) {
  int n = vec.size();
  int b = 0;
  int e = n - 1;
  while (b < e) {
    int mid = b + (e - b) / 2;
    if ((mid == b && vec[mid] < vec[mid + 1]) ||
        (mid == e && vec[mid] < vec[mid - 1]) ||
        (vec[mid] < vec[mid - 1] && vec[mid] < vec[mid + 1])) {
      return vec[mid];
    } else {
      if (vec[mid] > vec[e]) b = mid + 1;
      // this can not pass 2,2,0,0,1,1, if add the following line.
      // else if (vec[mid] < vec[e]) e = mid - 1;
      else e = e - 1;
    }
  }
  return vec[b];
}

}  // namespace algorithm

namespace Best {
// 一定要和最后一个比较，否则的话处理不了一直递增的情况，如果和第一个比较，一直递增就判断错了
int findMin(vector<int>& nums) {
    int n = nums.size();
    int b = 0;
    int e = n - 1;
    
    while (b < e) {
        int mid = b + (e - b) / 2;
        if (mid == b) {
            if (nums[mid] < nums[mid + 1]) return nums[mid];
            else b = mid + 1;
        } else {
            if (nums[mid] < nums[mid - 1] && nums[mid] <= nums[mid + 1]) {
                return nums[mid];
            } else {
                if (nums[mid] > nums[e]) b = mid + 1;
                else if (nums[mid] < nums[e]) e = mid - 1;
                else e--;
            }
        }
    }
    return nums[b];
}
}  // namespace Best

namespace memory {
int findMin(vector<int>& nums) {
    int n = nums.size();
    int b = 0;
    int e = n - 1;
    while (b < e) {
        if (nums[b] < nums[e]) return nums[b];
        else if (nums[b] == nums[e]) e--;
        else {
          int mid = b + (e - b) / 2;
          if (nums[mid] > nums[e]) b = mid + 1;
          else if (nums[mid] < nums[e]) e = mid;
          else e--;
        }
    }
    return nums[b];
}
}  // namespace memory

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(4);
  vec.push_back(5);
  vec.push_back(6);
  vec.push_back(7);
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(2);
  LOG(INFO) << FindMin(vec);
  vec.clear();
  vec.push_back(4);
  vec.push_back(4);
  vec.push_back(6);
  vec.push_back(6);
  vec.push_back(0);
  vec.push_back(0);
  vec.push_back(4);
  LOG(INFO) << FindMin(vec);
  vec.clear();
  vec.push_back(2);
  vec.push_back(2);
  vec.push_back(0);
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(1);
  LOG(INFO) << FindMin(vec);
  vec.clear();
  return 0;
}
