// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-01 18:36:46
// File  : code.cc
// Brief :

/*
Given an unsorted array of integers, find the length of the longest consecutive elements sequence.

For example,
Given [100, 4, 200, 1, 3, 2],
The longest consecutive elements sequence is [1, 2, 3, 4]. Return its length: 4.

Your algorithm should run in O(n) complexity.


*/


/*
 *   -------          -------------              ---------------
 *
 *   每次只只需要更新 连续数组的lower和upper既可以
 *   contiune 是关键
 *
 * */

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 15/15 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 140 milli secs
 * Progress: 67/67 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

int Merge(std::map<int, int> & dp, int left, int right) {
  int lower = left - dp[left]  + 1;
  int upper = right + dp[right] - 1;
  int len = upper - lower + 1;
  dp[upper] = len;
  dp[lower] = len;
  return len;
}

int LongestConsecutiveNB(std::vector<int> & num) {
  std::map<int, int> dp;  // dp[i]表示以i开始的或结束的连续串的长度, 只有边界的值才是有意义的，所以可以判断出dp[i]
                          //  表示的是开始位置还是结束位置
  int max =  1;
  for (int i = 0; i < num.size(); i++) {
    if (dp.count(num[i])) continue;  // 精髓
    dp[num[i]] = 1;
    if (dp.count(num[i] - 1)) {
      max = std::max(Merge(dp, num[i] - 1, num[i]), max);
    }
    if (dp.count(num[i] + 1)) {
      max = std::max(Merge(dp, num[i], num[i] + 1), max);
    }
  }
  return max;
}


/*
 * there is a bug in this function, but i can not find it.
 * */

int LongestConsecutive(std::vector<int> & num) {
  std::map<int, int> dp;
  int max = -1;
  for (int i = 0; i < num.size(); i++) {
    if (dp.count(num[i])) continue;  // NOte
    int t = 1;
    LOG(INFO) << JoinKeysValues(&dp);
    if (dp.count(num[i] - 1) && dp.count(num[i] + 1)) {
      // HERE(INFO);
      int tmp  = dp[num[i] - 1] + dp[num[i] + 1];
      int lower = num[i] - dp[num[i] - 1];
      int upper = num[i] + dp[num[i] + 1];
      dp[lower] = tmp + 1;
      dp[upper] = tmp + 1;
      t= dp[upper];
    } else if (dp.count(num[i] - 1)) {
      // HERE(INFO);
      int tmp = dp[num[i] - dp[num[i] - 1]];
      int lower = num[i] - dp[num[i] - 1];
      int upper = num[i];
      dp[lower] = tmp + 1;
      dp[upper] = tmp + 1;
      t= dp[upper];
      // LOG(INFO) << "lower:" << lower << " upper:" << upper;
      // LOG(INFO) << "dp[" << lower << "]:" << dp[lower] << " dp[" << upper << "]:" << dp[upper];
    } else if (dp.count(num[i] + 1)) {
      // HERE(INFO);
      int tmp = dp[num[i] + dp[num[i] + 1]];
      int lower = num[i];
      int upper = num[i] + dp[num[i] + 1];
      dp[lower] = tmp + 1;
      dp[upper] = tmp + 1;
      t = dp[upper];
      // canot use dp[num[i] + dp[num[i] + 1]] += 1 p[num[i]] = dp[num[i] + dp[num[i] + 1]]
      // dp[num[i] + 1] may == dp[num[i] + dp[num[i] + 1]] leading to a bug
    } else {
      // HERE(INFO);
      dp[num[i]] = t;
    }
    LOG(INFO) << "dp[" << num[i] << "]:" << dp[num[i]];
    max = std::max(max, t);
  }
  return max;
}


}  // namespace algorithm

using namespace algorithm;

namespace twice {
int LongestConsecutive(std::vector<int> & v) {
  std::map<int, int> dp;
  int max = 0;
  for (int i = 0; i < v.size(); i ++) {
    int t = v[i];
    if (dp.count(t)) continue;
    dp[t] = 1;
    max = std::max(max, dp[t]);
    if (dp.count(t - 1)) {
      int low = t - dp[t - 1];
      int len = dp[t - 1] + 1;
      dp[low] = len;
      dp[t] = len;
      max = std::max(max, len);
    }
    if (dp.count(t + 1)) {
      int low = t - dp[t] + 1;
      int up = t + dp[t + 1];
      int len = dp[t] + dp[t + 1];
      dp[low] = len;
      dp[up] = len;
      max = std::max(max, len);
    }
  }
  return max;
}
}  // namespace twice


int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(4);
  foo.push_back(0);
  foo.push_back(-4);
  foo.push_back(-2);
  foo.push_back(2);
  foo.push_back(5);
  foo.push_back(2);
  foo.push_back(0);
  foo.push_back(-8);
  foo.push_back(-8);
  foo.push_back(-8);
  foo.push_back(-1);
  foo.push_back(7);
  foo.push_back(4);
  foo.push_back(5);
  foo.push_back(5);
  foo.push_back(-4);
  foo.push_back(6);
  foo.push_back(6);
  foo.push_back(-3);
  LOG(INFO) << LongestConsecutive(foo);
  LOG(INFO) << LongestConsecutiveNB(foo);
  LOG(INFO) << twice::LongestConsecutive(foo);
  return 0;
}
