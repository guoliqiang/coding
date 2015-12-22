// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 16:32:10
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::vector<std::string> SummaryRanges(const std::vector<int>& nums) {
  std::vector<std::string> ans;
  int n = nums.size();
  if (n == 0) return ans;

  char buff[100];
  int begin = nums[0];
  int offset = 0;
  for (int i = 1; i < n; i++) {
    if (nums[i] == nums[i - 1] + 1) {
      offset++;
    } else {
      memset(buff, 0, sizeof(buff));
      if (offset > 0) {
        sprintf(buff, "%d->%d", begin, begin + offset);
      } else {
        sprintf(buff, "%d", begin);
      }
      ans.push_back(std::string(buff));
      begin = nums[i];
      offset = 0;
    }
  }

  memset(buff, 0, sizeof(buff));
  if (offset > 0) {
    sprintf(buff, "%d->%d", begin, begin + offset);
  } else {
    sprintf(buff, "%d", begin);
  }
  ans.push_back(std::string(buff));
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(-1);
  LOG(INFO) << JoinVector(SummaryRanges(vec));
  for (int j = 0; j < 1000; j++) {
  vec.clear();
  for (int  i = 0; i < 100; i++) vec.push_back(i);
  LOG(INFO) << JoinVector(SummaryRanges(vec));
  }
  vec.clear();
  LOG(INFO) << JoinVector(SummaryRanges(vec));
  return 0;
}
