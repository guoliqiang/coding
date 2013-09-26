// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 13:31:36
// File  : code.cc
// Brief :
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


int main(int argc, char** argv) {
  // int A[] = {2,3,1,1,4};
  // int A[] = {3,2,1,0,4};
  int A[] = {0,2,3};
  int size = sizeof(A) / sizeof(int);
  LOG(INFO) << CanJump(A, size);
  return 0;
}
