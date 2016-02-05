// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-11-21 02:44:39
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int FindMin(std::vector<int> & v) {
  int n = v.size();
  if (n == 1) return v[0];
  if (v[0] < v[1] && v[0] < v[n - 1]) return v[0];
  if (v[n - 1] < v[0] && v[n - 1] < v[n - 2]) return v[n - 1];
  int b = 0;
  int e = n - 1;
  while (b < e) {
    int mid = b + (e - b) / 2;
    // LOG(INFO) << b << " " << e << " " << mid;
    if (v[mid] < v[mid + 1] && v[mid] < v[mid - 1]) return v[mid];
    else {
      if (v[mid] > v[e]) b = mid + 1;
      else e = mid - 1;
    }
  }
  return v[b];
}

// 146 / 146 test cases passed.
// Status: Accepted
// Runtime: 52 ms
int FindMin2(std::vector<int> & v) {
  int n = v.size();
  int b = 0;
  int e = n - 1;
  while (b < e) {
    int mid = b + (e - b) / 2;
    if ((mid == b && v[mid] < v[mid + 1]) ||
        (mid == e && v[mid] < v[mid - 1]) ||
        (v[mid] < v[mid + 1] && v[mid] < v[mid - 1])) return v[mid];
    else {
      if (v[mid] > v[e]) b = mid + 1;
      else e = mid - 1;
    }
  }
  return v[b];
}
}  // namespace algorithm

using namespace algorithm;

int findMin(std::vector<int>& nums) {
        int n = nums.size();
        int b = 0;
        int e = n - 1;
        if (n == 1) return nums[0];
        if (nums[0] < nums[n - 1]) return nums[0];
        
        while (b <= e) {
            int mid = b + (e - b) / 2;
            if (mid == b && mid == e) return nums[mid];
            
            if (mid == e) {
                if (nums[mid] < nums[mid - 1]) return nums[mid];
                else {
                  e = mid - 1;
                  continue;
                }
            }
            
            if (mid == b) {
              if (nums[mid] < nums[mid + 1]) return nums[mid];
              else {
                b = mid + 1;
                continue;
              }
            }
            
            if (nums[mid] < nums[mid - 1] && nums[mid] < nums[mid + 1]) return nums[mid];
            else if (nums[mid] > nums[e]) b = mid + 1;
            else e = mid - 1;
        }
        return 0;
    }

namespace best {
    int findMin(std::vector<int>& nums) {
        int n = nums.size();
        int b = 0;
        int e = n - 1;
        while (b < e) {
            int mid = b + (e - b) / 2;
            if (mid == b) {
                if (nums[mid] < nums[mid + 1]) return nums[mid];
                else b = mid + 1;
            } else {
                if (nums[mid] < nums[mid + 1] && nums[mid] < nums[mid - 1]) return nums[mid];
                else if (nums[mid] > nums[e]) b = mid + 1;
                else e = mid - 1;
            }
        }
        return nums[b];
    }
}  // namespace best


namespace memory {
int findMin(std::vector<int>& nums) {
    int n = nums.size();
    int b = 0;
    int e = n - 1;
    while (b < e) {
        if (nums[b] < nums[e]) return nums[b];

        int mid = b + (e - b) / 2;
        if (nums[mid] > nums[e]) b = mid + 1;
        else e = mid;
     }
     return nums[b];
   }
}  // namespace memory

namespace extend {
/*
没问题
while (b < e) {
  int mid = b + (e - b) / 2;
  if ... b = mid + 1;
  else e = mid;
}

可能死循环
b = 0;
e = 1;
mid = 0;

while (b < e) {
  int mid = b + (e - b) / 2;
  if ... b = mid;
  else e = mid - 1;
}


*/
// 已知数组中某个元素，找这个元素最左最右位置，即最左边等于此值的位置，最右边等于此值的位置
int Left(std::vector<int> & vec, int v) {
  int b = 0;
  int e = vec.size() - 1;
  // 此位置一定存在
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (vec[mid] < v) b = mid + 1;
    else e = mid - 1;
  }
  return b;
}

int Right(std::vector<int> & vec, int v) {
  int b = 0;
  int e = vec.size() - 1;
  // 此位置一定存在
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (vec[mid] <= v) b = mid + 1;
    else e = mid - 1;
  }
  return b - 1;
}

int Left2(std::vector<int> & vec, int v) {
  int b = 0;
  int e = vec.size() - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (mid == b) {
      if (vec[mid] == v) return mid;
      else b = mid + 1;
    } else {
      if (vec[mid] == v && vec[mid - 1] < v) return mid;
      else if (vec[mid] < v) b = mid + 1;
      else e = mid - 1;
    }
  }
  return -1;
}

int Right2(std::vector<int> & vec, int v) {
  int b = 0;
  int e = vec.size() - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (mid == e) {
      if (vec[mid] == v) return mid;
      else e = mid - 1;
    } else {
      if (vec[mid] == v && vec[mid + 1] > v) return mid;
      else if (vec[mid] <= v) b = mid + 1;
      else e = mid - 1;
    }
  }
  return -1;
}

}  // namespace extend


namespace extend_nb {
int Left(std::vector<int> & vec, int v) {
  int b = 0;
  int e = vec.size() - 1;
  while (b < e) {
    int mid = b + (e - b) / 2;
    if (vec[mid] < v) b = mid + 1;
    else e = mid;
  }
  return b;
}

int Right(std::vector<int> & vec, int v) {
  int b = 0;
  int e = vec.size() - 1;
  while (b < e) {
    int mid = b + (e - b + 1) / 2;
    if (vec[mid] > v) e = mid - 1;
    else b = mid;
  }
  return b;
}
}

int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(0);
  v.push_back(1);
  v.push_back(1);
  v.push_back(1);
  v.push_back(1);
  v.push_back(2);
  LOG(INFO) << extend::Left(v, 2) << " " << extend::Right(v, 2);
  LOG(INFO) << extend_nb::Left(v, 3) << " " << extend_nb::Right(v, 3);
  LOG(INFO) << extend_nb::Left(v, -1) << " " << extend_nb::Right(v, -1);
  LOG(INFO) << extend::Left2(v, 3) << " " << extend::Right2(v, 3);
}
