// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 10:36:41
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int MajorityElement(std::vector<int>& nums) {
  int cnt = 1;
  int cur = nums[0];
  for (int i = 1; i < nums.size(); i++) {
    if (cur == nums[i]) {
      cnt++;
    } else {
      cnt--;
      if (cnt == 0) {
        cur = nums[i];
        cnt++;
      }
    }
  }
  return cur;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
