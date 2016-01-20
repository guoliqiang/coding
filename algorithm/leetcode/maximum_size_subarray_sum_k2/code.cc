// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-06 22:49:03
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int MaxSubArrayLen(std::vector<int> & nums, int k) {
  std::map<int, int> idx;
  idx[0] = -1;
  int max = 0;
  int sum = 0;
  for (int i = 0; i < nums.size(); i++) {
    sum += nums[i];
    if (!idx.count(sum)) {
      idx[sum] = i;
    }
    int rest = sum - k;
    for (std::map<int, int>::iterator it = idx.begin(); it != idx.end(); it++) {
      if (it->first > rest) break;
      max = std::max(max, i - it->second);
    }
  }

  return max;
}
}  // namespace algorithm

using namespace algorithm;

namespace msort {

void Sort(std::vector<std::pair<int, int> > & vec, int b, int e, int val, int & ans) {
  if (b >= e) return;
  int mid = b + (e - b) / 2;
  Sort(vec, b, mid, val, ans);
  Sort(vec, mid + 1, e, val, ans);

  std::vector<int> dp(e - b + 1, 0);
  for (int i = e; i >= mid + 1; i--) {
    if (i == e) dp[i] = vec[i].second;
    else dp[i] = std::max(dp[i - 1], vec[i].second);
  }

  int idx1 = b;
  int idx2 = mid + 1;
  int k = mid + 1;
  std::vector<std::pair<int, int> > tmp;

  while (idx1 <= mid || idx2 <= e) {
    if (idx2 > e || (idx1 <= mid && vec[idx1].first < vec[idx2].first)) {
      while (k <= e && vec[k].first < vec[idx1].first + val) k++;
      if (k <= e) {
        ans = std::max(ans, dp[k] - vec[idx1].second);
      }
      tmp.push_back(vec[idx1]);
      idx1++;
    } else {
      tmp.push_back(vec[idx2]);
      idx2++;
    }
  }
  for (int i = 0; i < tmp.size(); i++) vec[b + i] = tmp[i];
}

int MaxSubArrayLen(std::vector<int> & vec, int val) {
  int n = vec.size();
  if (n == 0) return 0;
  int ans = 0;
  std::vector<std::pair<int, int> > sum;
  for (int i = 0; i < n; i++) {
    if (i == 0) sum.push_back(std::make_pair(vec[i], i));
    else sum.push_back(std::make_pair(sum.back().first + vec[i], i));

    if (sum.back().first >= val) ans = std::max(ans, i + 1);
  }
  Sort(sum, 0, n - 1, val, ans);
  return ans;
}
}  // namespace msort

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(-1);
  vec.push_back(-4);
  vec.push_back(-2);
  vec.push_back(2);
  LOG(INFO) << MaxSubArrayLen(vec, 3);
  LOG(INFO) << msort::MaxSubArrayLen(vec, 3);
  vec.clear();
  vec.push_back(-2);
  vec.push_back(-1);
  vec.push_back(2);
  vec.push_back(2);
  LOG(INFO) << MaxSubArrayLen(vec, 1);
  LOG(INFO) << msort::MaxSubArrayLen(vec, 1);
  return 0;
}
