// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-22 09:48:33
// File  : code.cc
// Brief :

#include <vector>
#include <algorithm>
#include "base/public/common_ojhead.h"

/*
Given an array S of n integers, find three integers in S such that the sum is closest to a given number, target.
Return the sum of the three integers. You may assume that each input would have exactly one solution.
For example, given array S = {-1 2 1 -4}, and target = 1.

The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).

* */

/*
 * 
 * Run Status: Accepted!
 * Program Runtime: 0 milli secs
 * Progress: 20/20 test cases passed.
 *
 * Run Status: Accepted!
 * Program Runtime: 52 milli secs
 * Progress: 119/119 test cases passed.
 *
 * */

namespace algorithm {

// O(n^2)
int ThreeSumClosest(std::vector<int> & num, int target) {
  std::sort(num.begin(), num.end());
  int min = 0x7fffffff;
  int rs = 0;
  for (int i = 0; i < num.size(); i++) {
    int j = i + 1;
    int k = num.size() - 1;
    while (j < k) {
      int threesum = num[i] + num[j] + num[k];
      if (std::abs(threesum - target) < min) {
        min = std::min(min, std::abs(threesum - target));
        rs = threesum;
      }
      if (threesum < target) j++;
      else if (threesum > target) k--;
      else return target;
    }
  }
  return rs;
}
}  // namespace algorithm

namespace twice {
using namespace std;

int threeSumClosest(vector<int> &num, int target) {
  std::sort(num.begin(), num.end());
  int n = num.size();
  int min = num[0] + num[1] + num[2];
  for (int i = 0; i < n - 2; i++) {
    int b = i + 1;
    int e = n - 1;
    while (b < e) {
      int sum = num[i] + num[b] + num[e];
      if (sum == target) return sum;
      else if (sum > target) e--;
      else b++;
      if (abs(sum - target) < abs(min - target)) min = sum;
    }
  }
  return min;
}
}  // namespace twice

namespace analyze {
void Foo(std::vector<int> vec) {
  std::sort(vec.begin(), vec.end());
  std::vector<int> cur;
  for (int i = 0; i < vec.size(); i++) {
    for (int j = i + 1; j < vec.size(); j++) {
      cur.push_back(vec[i] + vec[j]);
    }
  }
  std::sort(cur.begin(), cur.end());
  LOG(INFO) << JoinVector(cur);
}

void Bar(std::vector<int> vec, int t) {
  std::sort(vec.begin(), vec.end());
  int b = 0;
  int e = vec.size() - 1;
  while (b < e) {
    int c = vec[b] + vec[e];
    if (c < t) b++;
    else e--;
    LOG(INFO) << c;
  }
}
}  // namespace analyze

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(-1);
  foo.push_back(2);
  foo.push_back(1);
  foo.push_back(-4);
  foo.push_back(15);
  foo.push_back(16);
  foo.push_back(27);
  analyze::Foo(foo);
  analyze::Bar(foo, 7);
  return 0;
}
