// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 21:03:08
// File  : code.cc
// Brief :

/*
Follow up for "Search in Rotated Sorted Array":
What if duplicates are allowed?
Would this affect the run-time complexity? How and why?
Write a function to determine if a given target is in the array.
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 110/110 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 48 milli secs
 * Progress: 271/271 test cases passed.
 *
 * */

/*
 * 先判断mid的位置，然后判断t在有序一侧的条件
 * 然后进入else的部分就是t在旋转有序的一侧
 * */

#include "base/public/common_head.h"

namespace algorithm {
bool BSearch (int * a, int b, int e, int t) {
  if (b > e) return false;
  if (b == e) return a[b] == t;
  
  int mid = b + (e - b) / 2;
  if (a[mid] == t) return true;
  if (a[mid] > a[e]) {
     if (t > a[e] && t < a[mid])
       return BSearch(a, b, mid - 1, t);
     else
       return BSearch(a, mid + 1, e, t);
  } else if (a[mid] < a[e]) {
    if (t <= a[e] && t > a[mid])  // Note here is <=
      return BSearch(a, mid + 1, e, t);
     else
       return BSearch(a, b, mid - 1, t);
  } else {
    return BSearch(a, b, e - 1, t);
  }
}

bool BSearch(int * a, int n, int t) {
  return BSearch(a, 0, n - 1, t);
}

}  // namespace algorithm

namespace twice {
bool BSearch(int * A, int n, int t) {
  int b = 0;
  int e = n - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (A[mid] == t) return true;
    else if (A[mid] > A[e]) {
      if (t > A[e] && t < A[mid]) e = mid - 1;
      else b = mid + 1;
    } else if (A[mid] < A[e]) {
      if (t <= A[e] && t > A[mid]) b = mid + 1;
      else e = mid - 1;
    } else {
      e--;
    }
  }
  return false;
}
}  // namespace twice

using namespace algorithm;

// 先找最小的位置然后做标准的二分查找
namespace clear {
using namespace std;

int FindMinIdx(std::vector<int> & nums) {
    int n = nums.size();
    int b = 0;
    int e = n - 1;
    while (b < e) {
        if (nums[b] < nums[e]) return b;
        else if (nums[b] == nums[e]) b++;
        else {
            int mid = b + (e - b) / 2;
            if (nums[mid] > nums[e]) b = mid + 1;
            else if (nums[mid] < nums[e]) e = mid;
            else e--;
        }
    }
    return b;
}

class Solution {
public:
    bool search(vector<int>& nums, int target) {
        int n = nums.size();
        if (n == 0) return false;
        
        int min_idx = FindMinIdx(nums);
        int b = target <= nums[n - 1] ? min_idx : 0;
        int e = target <= nums[n - 1] ? n - 1 : min_idx - 1;
        
        while (b <= e) {
            int mid = b + (e - b) / 2;
            if (nums[mid] == target) return true;
            else if (nums[mid] < target) b = mid + 1;
            else e = mid - 1;
        }
        return false;
    }
};
}  // namespace clear

int main(int argc, char** argv) {
  int A[] = {3,3,3,3,4,5,6,7,0,1,2,3,3,3,3};
  int size = sizeof(A) / sizeof(int);
  LOG(INFO) << BSearch(A, size, 5);
  LOG(INFO) << BSearch(A, size, 10);
  LOG(INFO) << BSearch(A, size, 3);
  LOG(INFO) << BSearch(A, size, 0);
  return 0;
}
