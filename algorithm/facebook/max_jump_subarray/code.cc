// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-30 16:45:36
// File  : code.cc
// Brief :
/*
Given a sequence of numbers A(1) ..A(n), find the continuous subsequenceA(i)..A(j) for which the sum of elements is maximum. 
condition: we should not select two contiguous numbers
http://www.careercup.com/question?id=23594662
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
int Dp(std::vector<int> & v) {
  int n = v.size();
  std::vector<int> dp(n, 0);
  if (n == 0) return 0;
  else if (n == 1) return v[0];
  else if (n == 2) return std::max(v[0], v[1]);
  else {
    dp[0] = v[0];
    dp[1] = std::max(v[0], v[1]);
    int rs = std::max(dp[0], dp[1]);
    int max = dp[0];
    // max值的妙用，将n^2的时间复杂度降低为O(n)
    for (int i = 2; i < n; i++) {
      dp[i] = std::max(v[i], v[i] + max);
      max = std::max(max, dp[i - 1]);
      rs = std::max(rs, dp[i]);
    }
    return rs;
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  // -50,7,11,2,-1,3,4
  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  v.push_back(6);
  LOG(INFO) << Dp(v);
  return 0;
}
