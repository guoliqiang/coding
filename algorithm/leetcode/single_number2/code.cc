// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-11 13:39:56
// File  : code.cc
// Brief :

/*
Given an array of integers, every element appears three times except for one. Find that single one.
Note:
Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?
*/

/*
 *
 * 11 / 11 test cases passed.
 * Status: Accepted
 * Runtime: 48 ms
 * Submitted: 0 minutes ago
 *
 * */

// http://www.quora.com/Algorithms/Given-an-integer-array-such-that-every-element-occurs-3-times-except-one-element-which-occurs-only-once-how-do-I-find-that-single-element-in-O-1-space-and-O-n-time-complexity#
// http://stackoverflow.com/questions/7338070/finding-an-element-in-an-array-where-every-element-is-repeated-odd-number-of-tim
// http://stackoverflow.com/questions/7338437/finding-an-element-in-an-array-that-isnt-repeated-a-multiple-of-three-times
//
#include "base/public/common_head.h"

namespace algorithm {
int SingleNumber(int * A, int n) {
  int one = 0;  // 保存那些出现一次的bit
  int two = 0;  // 保存那些出现两次的bit
  for (int i = 0; i < n; i++) {
    two |= one & A[i];  // 更新那些出现两个的bit
    one ^= A[i];        // 更新那些出现一次的bit
    int three = one & two;
    one &= ~(three);    // 如果一个bit出现三次，它需要从出现一次中剔除
    two &= ~(three);    // 如果一个bit出现三次，它需要从出现两次中剔除
  }
  return one;  // 出现两次 return two;
}

int SingleNumber2(int * A, int n) {
  std::vector<int> num(32, 0);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 32; j++) {
      num[j] = (num[j] + ((A[i] >> j) & 1)) % 3;
    }
  }
  int rs = 0;
  for (int i = 0; i < 32; i++) {
    if (num[i]) rs |= 1 << i;
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
int SingleNumber(int A[], int n) {
  int one = 0;
  int two = 0;
  for (int i = 0; i < n; i++) {
    int three = two & A[i];
    two |= (one & A[i]);
    one |= A[i];
    two ^= three;
    one ^= three;
    /* or
    two &= ~three;
    one &= ~three;
    */
  }
  return one;
}
}  // namespace twice


int main(int argc, char** argv) {
  int A[] = {3, 3, 3, 9};
  LOG(INFO) << SingleNumber(A, sizeof(A) / sizeof(int));
  LOG(INFO) << SingleNumber2(A, sizeof(A) / sizeof(int));
  return 0;
}
