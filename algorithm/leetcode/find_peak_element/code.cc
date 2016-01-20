// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-15 20:21:36
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

int FindPeakElement(std::vector<int> & nums) {
 for (int i = 0; i < nums.size(); i++) {
   if (i == 0 && i == nums.size() - 1) {
     return i;
   } else if (i == 0) {
     if (nums[i] > nums[i + 1]) return i;
   } else if (i == nums.size() - 1) {
     if (nums[i] > nums[i - 1]) return i;
   } else {
     if (nums[i] > nums[i - 1] && nums[i] > nums[i + 1]) return i;
   }
 }
 return 0;
}

namespace NB {
int FindPeakElement(std::vector<int> & nums) {
  int n = nums.size();
  int b = 0;
  int e = n - 1;
  // 走到了边界依然需要通过判断左边或者右边，需要这样的写法，前提是
  // 一定得存在有结果在里面
  // 通常的拍完序的做二分查找，走到边界，仅仅依靠当前值和target值就可以判断出来
  while (b < e) {
    int mid = b + (e - b) / 2;
    if ((mid == b && nums[mid] > nums[mid + 1]) ||
        (mid == e && nums[mid] > nums[mid - 1]) ||
        (nums[mid] > nums[mid - 1] && nums[mid] > nums[mid + 1])) {
      return mid;
    } else if (mid == b) {
      b = mid + 1;
    } else if (mid == e) {
      e = mid - 1;
    } if (nums[mid] < nums[mid - 1]) {
      e = mid - 1;
    } else {
      b = mid + 1;
    }
  }
  return b;
}

int FindPeakElement2(std::vector<int> & nums) {
  int n = nums.size();
  int b = 0;
  int e = n - 1;
  while (b < e) {
    int mid = b + (e - b) / 2;
    // mid 不可能等于e
    if (mid == b) {
      if (nums[mid] > nums[mid + 1]) return mid;
      else b = mid + 1;
    } else {
      if (nums[mid] > nums[mid + 1] && nums[mid] > nums[mid - 1]) return mid;
      else if (nums[mid] > nums[mid + 1]) e = mid - 1;
      else b = mid + 1;
    }
  }
  return b;
}

}  // namespace NB

int FindSmaller(std::vector<int> & vec, int target) {
  int n = vec.size();
  int b = 0;
  int e = n - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (mid == e) {
      if (vec[mid] < target) return mid;
      else e = mid - 1;
    } else if (vec[mid] < target && vec[mid + 1] >= target) {
      return mid;
    } else if (vec[mid] < target) b = mid + 1;
    else e = mid - 1;
  }
  return -1;
}

int FindSmaller2(std::vector<int> & vec, int target) {
  int n = vec.size();
  int b = 0;
  int e = n - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (vec[mid] < target) {
      b = mid + 1;
    } else {
      e = mid - 1;
    }
  }
  return e;
}

int FindGreater(std::vector<int> & vec, int target) {
  int n = vec.size();
  int b = 0;
  int e = n - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (mid == b) {
      if (vec[mid] > target) return mid;
      else b = mid + 1;
    } else if (vec[mid] > target && vec[mid - 1] <= target) {
      return mid;
    } else if (vec[mid] > target) {
      e = mid - 1;
    } else {
      b = mid + 1;
    }
  }
  return vec.size();
}

int FindGreater2(std::vector<int> & vec, int target) {
  int n = vec.size();
  int b = 0;
  int e = n - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (vec[mid] <= target) b = mid + 1;
    else e = mid - 1;
  }
  return b;
}

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(3);
  vec.push_back(3);
  vec.push_back(4);
  vec.push_back(10);
  vec.push_back(15);
  LOG(INFO) << JoinVector(vec);
  LOG(INFO) << FindSmaller(vec, 15) << " " << FindSmaller2(vec, 15);
  LOG(INFO) << FindSmaller(vec, 4) << " " << FindSmaller2(vec, 4);
  LOG(INFO) << FindSmaller(vec, 3) << " " << FindSmaller2(vec, 3);
  LOG(INFO) << FindGreater(vec, 3) << " " << FindGreater2(vec, 3);
  LOG(INFO) << FindGreater(vec, 10) << " " << FindGreater2(vec, 10);
  LOG(INFO) << FindGreater(vec, 15) << " " << FindGreater2(vec, 15);
  return 0;
}
