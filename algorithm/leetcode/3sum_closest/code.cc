// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-22 09:48:33
// File  : code.cc
// Brief :

#include <vector>
#include <algorithm>
#include "base/public/logging.h"

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

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(-1);
  foo.push_back(2);
  foo.push_back(1);
  foo.push_back(-4);
  LOG(INFO) << ThreeSumClosest(foo, 1);
  return 0;
}
