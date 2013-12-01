// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 23:06:37
// File  : code.cc
// Brief :

/*
Given an array of integers, every element appears twice except for one. Find that single one.
Note:
Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?
*/

/*
14 / 14 test cases passed.
Status: Accepted
Runtime: 56 ms
*/

#include "base/public/common_head.h"

namespace algorithm {

class Solution {
 public:
  int singleNumber(int A[], int n) {
    for (int i = 1; i < n; i++) {
      A[0] ^= A[i];
    }
    return A[0];
  }
};

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  return 0;
}
