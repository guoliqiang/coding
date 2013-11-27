// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 19:48:35
// File  : code.cc
// Brief :

/*
Given an array of non-negative integers, you are initially positioned at the first index of the array.
Each element in the array represents your maximum jump length at that position.
Your goal is to reach the last index in the minimum number of jumps.
For example:
Given array A = [2,3,1,1,4]
The minimum number of jumps to reach the last index is 2.
(Jump 1 step from index 0 to 1, then 3 steps to the last index.)
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 40/40 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 56 milli secs
 * Progress: 88/88 test cases passed.
 *
 * */

#include "base/public/logging.h"
#include "base/public/common_head.h"

namespace algorithm {
int Jump(int * a, int n) {
  std::vector<int> foo(n, 0x7fffffff);
  foo[0] = 0;
  for (int i = 0; i < n; i++) {
    if (i != 0 && a[i] < a[i - 1]) continue;  // 用于加速，否则会T
    for (int j = 1; j <= a[i]; j++) {
      if (i + j < n && foo[i] != 0x7fffffff &&
          foo[i + j] > foo[i] + 1) foo[i + j] = foo[i] + 1;
    }
  }
  return foo[n - 1];
}

int DP(int A[], int n) {
  std::vector<int> dp(n, -1);
  dp[0] = 0;
  int max = 0;
  for (int i = 0; i < n; i++) {
    for (int j = max + 1; j <= std::min(n - 1, A[i] + i); j++) {
        dp[j] = dp[i] + 1;
    }
    max = std::max(max, A[i] + i);
  }
  return dp[n - 1];
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  int A[] = {1, 2, 0, 1};
  int size = sizeof(A) / sizeof(int);
  LOG(INFO) << Jump(A, size);
  LOG(INFO) << DP(A, size);
  return 0;
}
