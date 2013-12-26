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

/*
之前的理解是错误的，位的计算方法都不能处理
既有一个出现一次，又有一个出现两此的情况
*/

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
// 不能处理出现即出现一次的，又出现两次的
int SingleNumber(int A[], int n) {
  int one = 0;  // 出现一次或两次的
  int two = 0;  // 仅仅出现两次的
  // 因为结果中没有出现两次的 one 最终就是出现一次的
  for (int i = 0; i < n; i++) {
    int three = two & A[i];
    two |= (one & A[i]);
    one |= A[i];
    // two ^= three;
    // one ^= three;
    // or
    two &= ~three;
    one &= ~three;
  }
  return one;
}

}  // namespace twice

namespace third {
int Single(int A[], int n) {
  int one = 0;  // 仅仅出现一次的
  int two = 0;  // 仅仅出现两次的
  for (int i = 0; i < n; i++) {
    int three = two & A[i];
    two |= one & A[i];  // 此时two里面包含出现两此或三次的, 三次的bit来源于之前two中的 one & A[i] 指定是只出现两次的 
    one |= A[i];        // 此时one里面包含出现一次两次或三次的, 因为A[i] 中可能某位此时出现三次, 两次
    
    one &= ~two;        // 此时one里面仅仅包含出现一次的 
    two &= ~three;        // 此时one里面仅仅包含出现两次的
    // or
    // two &= ~three;
    // one &= ~two;
    // one &+ ~three;
  }
  return one;
}
}  // namespace third

int main(int argc, char** argv) {
  // int A[] = {5, 5, 4, 8, 8, 8};
  int A[] = {4, 8, 8, 8};
  LOG(INFO) << SingleNumber(A, sizeof(A) / sizeof(int));
  LOG(INFO) << SingleNumber2(A, sizeof(A) / sizeof(int));
  LOG(INFO) << twice::SingleNumber(A, sizeof(A) / sizeof(int));
  LOG(INFO) << third::Single(A, sizeof(A) / sizeof(int));
  return 0;
}
