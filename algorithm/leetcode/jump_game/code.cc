// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 13:31:36
// File  : code.cc
// Brief :
/*
Given an array of non-negative integers, you are initially positioned at the first index of the array.
Each element in the array represents your maximum jump length at that position.
Determine if you are able to reach the last index.

For example:
A = [2,3,1,1,4], return true.

A = [3,2,1,0,4], return false.

*/
/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 29/29 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 44 milli secs
 * Progress: 66/66 test cases passed.
 *
 * */

#include <vector>
#include <algorithm>
#include "base/public/logging.h"

namespace algorithm {

/*
 * max_index 用于加速
 *
 * */
bool CanJump(int * a, int n) {
  if (n == 0) return true;
  std::vector<bool> dp(n, false);
  int max_index = 0;
  dp[0] = true;
  for (int i = 0; i < n -1; i++) {
    if (dp[i] == false) break;  // Note here, you need to judge can this site be reached?
    if (std::min(a[i] + i, n - 1) <= max_index) continue;
    else {
      for (int j= max_index + 1; j <= std::min(n - 1, a[i] + i); j++) {
        dp[j] = true;
      }
      max_index = std::min(n - 1, a[i] + i);
    }
  }
  return dp[n - 1];
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
using namespace std;
bool DP(int A[], int n) {
  int max = 0;
  std::vector<bool> dp(n, false);
  dp[0] = true;
  for (int i = 0; i < n; i++) {
      if (dp[i] == false) break;
      if (A[i] + i > max) {
          for (int j = max; j <= std::min(n - 1, A[i] + i); j++) dp[j] = true;
          max = A[i] + i;
      }
  }
  return dp[n - 1];
}

bool DP2(int A[], int n) {
  std::vector<bool> dp(n, false);
  dp[0] = true;
  int i = 0;
  while (i < n) {
      for (int j = i; j <= std::min(n - 1, A[i] + i); j++) dp[j] = true;
      i = A[i] + i;
      if (A[i] == 0) break;
  }
  return dp[n - 1];
}

bool DP3(int A[], int n) {
  int max = 0;
  std::vector<bool> dp(n, false);
  dp[0] = true;
  for (int i = 0; i < n; i++) {
      if (dp[i] == false) continue;
      for (int j = max; j <= std::min(n - 1, A[i] + i); j++) dp[j] = true;
      max = std::max(max, A[i] + i);
  }
  return dp[n - 1];
}
}  // namespace twice

namespace BFS {
bool CanJump(std::vector<int>& nums) {
  int n = nums.size();

  int left = 0;
  int right = 0;
  while (right != n - 1 && right >= left) {
      int t = right;
      for (int i = left; i <= right; i++) {
          t = std::max(t, i + nums[i]);
      }
      left = right + 1;
      right = std::min(t, n - 1);
  }
  return right == n - 1;
}
}  // namespace BFS

namespace NB {
bool canJump(std::vector<int>& nums) {
  int n = nums.size();
  int right = 0;
  for (int i = 0; i < n && i <= right; i++) {
      right = std::max(right, i + nums[i]);
  }
  return right >= n - 1;
}
}  // namespace NB

int main(int argc, char** argv) {
  // int A[] = {2,3,1,1,4};
  // int A[] = {3,2,1,0,4};
  int A[] = {0,2,3};
  int size = sizeof(A) / sizeof(int);
  LOG(INFO) << CanJump(A, size);
  return 0;
}
