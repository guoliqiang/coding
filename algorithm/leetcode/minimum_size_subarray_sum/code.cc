// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 15:25:39
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int MinSubArrayLen(int s, std::vector<int>& nums) {
  int b = 0;
  int e = 0;
  int sum = 0;
  int ans = 0;
  while (e < nums.size()) {
    sum += nums[e];
    if (sum >= s) {
      while (sum - nums[b] >= s) {
        sum -= nums[b];
        b++;
      }
      ans = (ans == 0) ? e - b + 1 : std::min(ans, e - b + 1);
    }
    e++;
  }
  return ans;
}
}  // namespace algorithm


// 可以处理有负数的情况
// 因为本题是全是正数，sum生成的时候就是一个有序的，因此可以直接用二分产找
namespace msort {

void Sort(std::vector<std::pair<int, int> > & vec, int b, int e, int val, int & ans) {
  if (b > e) return;
  if (b == e) {
    if (vec[b].first >= val) ans = std::min(ans, vec[b].second + 1);
    return;
  }

  int mid = b + (e - b) / 2;
  Sort(vec, b, mid, val, ans);
  Sort(vec, mid + 1, e, val, ans);

  int i = b;
  int j = mid + 1;
  int k = mid + 1;
  std::vector<std::pair<int, int> > tmp;
  std::vector<int> dp(vec.size(), 0);
  for (int m = e; m >= mid + 1; m--) {
    if (m == e) dp[m] = vec[m].second;
    else dp[m] = std::min(dp[m + 1], vec[m].second);
  }

  while (i <= mid || j <= e) {
    if (j > e || (i <= mid && vec[i].first < vec[j].first)) {
      while (k <= e && vec[k].first < vec[i].first + val) k++;
      if (k <= e) {
        // LOG(INFO) << dp[k] << " " << vec[i].second;
        ans = std::min(ans, dp[k] - vec[i].second);
      }
      tmp.push_back(vec[i]);
      i++;
    } else {
      tmp.push_back(vec[j]);
      j++;
    }
  }
  for (int m = 0; m < tmp.size(); m++) vec[b + m] = tmp[m];
}

int MinSubArrayLen(int s, std::vector<int>& nums) {
  int n = nums.size();
  std::vector<std::pair<int, int> > sum;
  for (int i = 0; i < n; i++) {
    if (i == 0) sum.push_back(std::make_pair(nums[i], i));
    else sum.push_back(std::make_pair(sum.back().first + nums[i], i));
  }
  int ans = INT_MAX;
  Sort(sum, 0, n - 1, s, ans);
  return ans == INT_MAX ? 0 : ans;
}
}  // namespace msort

namespace bs {
int Find(std::vector<int> & sum, int begin, int end, int val) {
  int b = begin;
  int e = end;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (sum[mid] == val) return mid;
    else if (sum[mid] > val) e = mid - 1;
    else {
      if (mid == e || sum[mid + 1] > val) return mid;
      else b = mid + 1;
    }
  }
  return -1;
}

int MinSubArrayLen(int s, std::vector<int>& nums) {
  int n = nums.size();
  std::vector<int> sum;
  sum.push_back(0);
  for (int i = 0; i < n; i++) {
    sum.push_back(sum.back() + nums[i]);
  }
  int ans = INT_MAX;
  for (int i = 1; i <= n; i++) {
    int idx = Find(sum, 0, i - 1, sum[i] - s);
    if (idx != -1) {
      ans = std::min(ans, i - idx);
    }
  }
  return ans == INT_MAX ? 0: ans;
}
}  // namespace bsearch

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(4);
  vec.push_back(3);
  LOG(INFO) << bs::MinSubArrayLen(7, vec);
  LOG(INFO) << MinSubArrayLen(7, vec);

  LOG(INFO) << MinSubArrayLen(10, vec);
  LOG(INFO) << msort::MinSubArrayLen(10, vec);
  LOG(INFO) << bs::MinSubArrayLen(10, vec);
  return 0;
}
