// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 18:16:49
// File  : code.cc
// Brief :

/*
Say you have an array for which the ith element is the price of a given stock on day i.

Design an algorithm to find the maximum profit.
You may complete as many transactions as you like (ie, buy one and sell one share of the stock multiple times).
However, you may not engage in multiple transactions at the same time (ie, you must sell the stock before you buy again).

Discuss

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 22/22 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 48 milli secs
 * Progress: 198/198 test cases passed.
 *
 * 不是DP是贪心
 * 能赚就赚，最后挣的一定是最多的
 * */

#include "base/public/common_head.h"

namespace NB {
int MaxProfit(std::vector<int> &prices) {
  int rs = 0;
  for (int i = 1; i < prices.size() ; i++) {
     int delta = prices[i] - prices[i-1];
     if (delta > 0) rs += delta;
  }
  return rs;
}

}  // namespace NB

namespace algorithm {

int MaxProfit(std::vector<int> & v) {
  if (v.size() < 1) return 0;
  int rs = 0;
  int i = 1;
  while (i < v.size()) {
    while (v[i] <= v[i - 1] && i < v.size()) i++;
    if (i == v.size()) break;
    int foo = v[i - 1];
    while(v[i] > v[i - 1] && i < v.size()) i++;
    rs += v[i - 1] - foo;
  }
  return rs;
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
