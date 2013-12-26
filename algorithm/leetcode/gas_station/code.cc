// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 20:01:28
// File  : code.cc
// Brief :

/*
There are N gas stations along a circular route, where the amount of gas at station i is gas[i].
You have a car with an unlimited gas tank and it costs cost[i] of gas to travel from station i to its next station (i+1).
You begin the journey with an empty tank at one of the gas stations.
Return the starting gas station's index if you can travel around the circuit once, otherwise return -1.
Note:
The solution is guaranteed to be unique.

*/

#include "base/public/logging.h"

/*
 * 12 / 12 test cases passed.
 * Status: Accepted
 * Runtime: 32 ms
 * Submitted: 0 minutes ago
 * */

/*
分为三种情况：
----|  max   |---  显然

|max1| --- |max2|
     i     j
sum最大时只能是i或j时，如果是i起始位置计算出来一定是j
                       如果是j起始位置也一定是j

*/

namespace algorithm {

int Can(std::vector<int> & gas, std::vector<int> & cost) {
  int sum = 0;
  int max  = 0x80000000;
  int max_id = 0;
  for (int i = 0; i < gas.size(); i++) {
    sum += (gas[i] - cost[i]);
    if (sum > max) {
      max = sum;
      max_id = i;
    }
  }
  if (sum < 0) return -1;
  
  sum = 0;
  int i = max_id;
  max = 0x80000000;
  int rs = 0;
  while (true) {
    sum += (gas[i] - cost[i]);
    if (sum > max) {
      max = sum;
      rs = i;
    }
    i--;
    if (i == -1) i = gas.size() - 1;
    if (i == max_id) break;
  }
  return rs;
}

/*
 * 会　Ｔ
 * */

int DP(std::vector<int> & gas, std::vector<int> & cost,
       std::vector<std::vector<int> > & dp) {
  int n = gas.size();
  int flag = false;
  for (int i = 0; i < n; i++) dp[flag][i] = gas[i] - cost[i];
  
  for (int k = 1; k <= n; k++) {
    for (int i = 0; i < n; i++) {
      int t = gas[(i + k) % n] - cost[(i + k) % n];
      if (dp[flag][i] >= 0) {
        dp[!flag][i] = dp[flag][i] + t;
      } else {
        dp[!flag][i] = std::min(dp[flag][i], t);
      }
    }
    flag = !flag;
  }
  return flag;
}

int CanCompleteCircuit(std::vector<int> & gas, std::vector<int> & cost) {
  int n = gas.size();
  std::vector<std::vector<int> > dp(2, std::vector<int>(n, 0));
  int idx = DP(gas, cost, dp);

  for (int i = 0; i < n; i++) {
    if (dp[idx][i] >= 0) return i;
  }
  return -1;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> gas;
  std::vector<int> cost;
  gas.push_back(1);
  gas.push_back(2);
  gas.push_back(3);
  gas.push_back(4);
  gas.push_back(5);
  cost.push_back(3);
  cost.push_back(4);
  cost.push_back(5);
  cost.push_back(1);
  cost.push_back(2);
  // cost.push_back(70);
  LOG(INFO) << CanCompleteCircuit(gas, cost);
  LOG(INFO) << Can(gas, cost);
  return 0;
}
