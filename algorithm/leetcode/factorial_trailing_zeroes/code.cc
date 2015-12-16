// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 11:15:36
// File  : code.cc
// Brief :

/*
Given an integer n, return the number of trailing zeroes in n!.
Note: Your solution should be in logarithmic time complexity.
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
int TrailingZeroes(int n) {
  int ans = 0;
  while (n > 0) {
    n /= 5;
    ans += n;
  }
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << TrailingZeroes(30);
  LOG(INFO) << TrailingZeroes(4);
  return 0;
}
