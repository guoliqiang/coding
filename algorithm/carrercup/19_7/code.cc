// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 13:33:35
// File  : code.cc
// Brief :

/*
 * You are given an array of integers (both positive and negative). 
 * Find the continuous sequence with the largest sum. Return the sum.
 * */

#include "base/public/common_head.h"

namespace algorithm {

std::vector<int> FindMaxContinuousSequence(std::vector<int> & v) {
  std::vector<int> dp(v.size(), 0);
  int idx = 0;
  int max = v[0];
  dp[0] = v[0];
  for (int i = 1; i < v.size(); i++) {
    dp[i] = std::max(v[i], dp[i - 1] + v[i]);
    if (dp[i] > max) {
      max = dp[i];
      idx = i;
    }
  }
  int t = 0;
  std::vector<int> rs;
  while (t != max) {
    rs.insert(rs.begin(), v[idx]);
    t += v[idx];
    idx--;
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(2);
  v.push_back(-8);
  v.push_back(3);
  v.push_back(-2);
  v.push_back(4);
  v.push_back(-10);
  std::vector<int> rs = FindMaxContinuousSequence(v);
  LOG(INFO) << JoinVector(rs);
  return 0;
}
