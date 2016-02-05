// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 21:52:34
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
// M S S S
//  L M M
//
//  S M M L 的结果只能是
//  M L M S
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

namespace my {
void WiggleSort(std::vector<int>& nums) {
        int n = nums.size();
        int mid = n / 2;
        nth_element(nums.begin(), nums.begin() + mid, nums.end());
        
        int b = 0;
        int e = n;
        int idx = -1;
        
        while (b < e) {
            if (nums[b] < nums[mid]) {
                std::swap(nums[++idx], nums[b++]);
            } else if (nums[b] > nums[mid]) {
                std::swap(nums[b], nums[--e]);
            } else {
                b++;
            }
        }
        std::vector<int> ans;
        int i = (n % 2 == 0) ? mid - 1 : mid;
        int j = n - 1;
        for (int k = 0; k < n / 2; k++) {
           ans.push_back(nums[i--]);
           ans.push_back(nums[j--]);
        }
        if (n % 2) ans.push_back(nums[i--]);
        nums = ans;
    }
}  // namespace my

namespace extend {
void WiggleSort(std::vector<int>& nums) {
        int n = nums.size();
        int mid = n / 2;
        nth_element(nums.begin(), nums.begin() + mid, nums.end());
        
        int b = 0;
        int e = n;
        int idx = -1;
        
        while (b < e) {
            if (nums[b] < nums[mid]) {
                std::swap(nums[++idx], nums[b++]);
            } else if (nums[b] > nums[mid]) {
                std::swap(nums[b], nums[--e]);
            } else {
                b++;
            }
        }
        std::vector<int> ans;
        int i = mid;
        int j = 0;
        for (int k = 0; k < n / 2; k++) {
           ans.push_back(nums[i++]);
           ans.push_back(nums[j++]);
        }
        if (n % 2) ans.push_back(nums[i++]);
        nums = ans;
    }
}  // namespace extend 
using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(4);
  vec.push_back(5);
  vec.push_back(5);
  vec.push_back(6);
  LOG(INFO) << JoinVector(vec);
  extend::WiggleSort(vec);
  LOG(INFO) << JoinVector(vec);
  return 0;
}
