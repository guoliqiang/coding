// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 10:48:28
// File  : code.cc
// Brief :

/*
Find the contiguous subarray within an array (containing at least one number) which has the largest sum.
For example, given the array [−2,1,−3,4,−1,2,1,−5,4],
the contiguous subarray [4,−1,2,1] has the largest sum = 6.

click to show more practice.
More practice:
If you have figured out the O(n) solution, try coding another solution using the divide and conquer approach, which is more subtle.

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 0 milli secs
 * Progress: 15/15 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 40 milli secs
 * Progress: 200/200 test cases passed.
 *
 * */

#include <vector>
#include "base/public/logging.h"

namespace algorithm {
int MaxSubArray(int * a, int n) {
  std::vector<int> dp(n, 0);
  int index = 0;
  dp[0] = a[0];
  int max = dp[0];
  for (int i = 1; i < n ; i++) {
    dp[i] = std::max(a[i], a[i] + dp[i - 1]);
    if (dp[i] > max) {
      max = dp[i];
      index = i;
    }
  }
  return max;
  // 数组开始位置
  while (max > 0) {
    max -= a[index];
    index--;
  }
  return index;
}

/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 15/15 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 52 milli secs
 * Progress: 200/200 test cases passed.
 *
 * */

int DivideConquer(int * a, int b, int e) {
  if ( b == e) return a[b];
  else {
    int mid = b + (e - b) / 2;
    int max1 = DivideConquer(a, b, mid);
    int max2 = DivideConquer(a, mid + 1, e);
    int max3 = 0;

    int max = 0x80000000;
    int foo = 0;
    for (int i = mid; i >= b; i--) {
      if (foo + a[i] > max) max = foo + a[i];
      foo += a[i];
    }
    max3 = max;
    foo = 0;
    max = 0x80000000;
    for (int i = mid + 1; i <= e; i++) {
      if (foo + a[i] > max) max = foo + a[i];
      foo += a[i];
    }
    max3 += max;
    return std::max(max3, std::max(max1, max2));
  }
}

int DivideConquer(int * a, int n) {
  return DivideConquer(a, 0, n - 1);
}

}  // namespace algorithm



namespace twice {
int MaxSubArray(int A[], int n) {
  int pre = A[0];
  int rs = pre;
  for (int i = 1; i < n; i++) {
    pre = std::max(pre + A[i], A[i]);
    rs = std::max(rs, pre);
  }
  return rs;
}
}  // namespace twice

using namespace algorithm;


int main(int argc, char** argv) {
  // int A[] = {1};
  // int A[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
  int A[] = {-2, -1};
  int size = sizeof(A) / sizeof(int);
  LOG(INFO) << MaxSubArray(A, size);
  LOG(INFO) << DivideConquer(A, size);
  return 0;
}
