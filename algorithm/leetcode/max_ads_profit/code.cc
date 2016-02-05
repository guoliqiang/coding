// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-21 19:04:08
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct Node {
  int start;
  int end;
  int profit;
};

int BSearch(const std::vector<Node> & vec, int end, int t) {
  int b = 0;
  int e = end;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (mid == e) {
      if (vec[mid].end <= t) return mid;
      else e = mid - 1;
    } else {
      if (vec[mid].end <= t && vec[mid + 1].end > t) return mid;
      else if (vec[mid].end <= t) b = mid + 1;
      else e = mid - 1;
    }
  }
  return -1;
}

bool Cmp(const Node & x, const Node & y) {
  return x.end < y.end;
}


int MaxProfit(std::vector<Node> & ads) {
  int n = ads.size();
  std::sort(ads.begin(), ads.end(), Cmp);
  std::vector<int> dp(n, 0);

  for (int i = 0; i < n; i++) {
    if (i == 0) dp[i] = ads[i].profit;
    else {
      int idx = BSearch(ads, i - 1, ads[i].start);
      int cur = idx != -1 ? dp[idx] + ads[i].profit : ads[i].profit;
      dp[i] = std::max(dp[i - 1], cur);
    }
  }
  return dp[n - 1];
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
