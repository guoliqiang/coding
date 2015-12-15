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

int main(int argc, char** argv) {
  return 0;
}
