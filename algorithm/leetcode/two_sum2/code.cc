// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-12 16:48:45
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::vector<int> TwoSum(std::vector<int> &numbers, int target) {
  int n = numbers.size();
  int i = 0;
  int j = n - 1;
  while (numbers[i] + numbers[j] != target) {
    if (numbers[i] + numbers[j] < target) i++;
    else j--;
  }
  std::vector<int> ans;
  ans.push_back(i + 1);
  ans.push_back(j + 1);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
