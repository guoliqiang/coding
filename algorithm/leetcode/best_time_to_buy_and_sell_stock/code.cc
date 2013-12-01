// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 17:45:29
// File  : code.cc
// Brief :

/*
Say you have an array for which the ith element is the price of a given stock on day i.
If you were only permitted to complete at most one transaction (ie, buy one and sell one
share of the stock), design an algorithm to find the maximum profit.

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 22/22 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 52 milli secs
 * Progress: 198/198 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace NB {
int GetBestTime(int * stocks, int n, int & buy, int & sell) {
  int min = 0;
  int maxDiff = 0;
  buy = 0;
  sell = 0;
  for (int i = 0; i < n; i++) {
    if (stocks[i] < stocks[min]) min = i;
     int diff = stocks[i] - stocks[min];
     if (diff > maxDiff) {
       buy = min;
       sell = i;
       maxDiff = diff;
    }
  }
  return maxDiff;
}
}  // namespace NB

// 这个思想很好～
namespace algorithm {

void DP(std::vector<int> & v, std::vector<int> & dp) {
  if (dp.size() > 1) {
   dp[1] = v[1] - v[0];
  }

  for (int i = 2; i < dp.size(); i++) {
    int t = v[i] - v[i - 1];
    if (t > dp[i- 1] + t) dp[i] = t;
    else dp[i] = dp[i - 1] + t;
  }
}

int MaxProfit(std::vector<int> & v) {
  if(v.size() <= 1) return 0;

  std::vector<int> dp(v.size(), 0);
  DP(v, dp);
  int max = 0x80000000;
  for (int i = 1; i < dp.size(); i++) {
    if (dp[i] > max) max = dp[i];
  }
  return max > 0 ? max : 0;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(1);
  foo.push_back(-1);
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(5);
  foo.push_back(3);
  foo.push_back(6);
  foo.push_back(10);
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  LOG(INFO) << MaxProfit(foo);
  return 0;
}
