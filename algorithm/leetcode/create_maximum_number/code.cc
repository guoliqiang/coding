// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-25 15:29:32
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::vector<int> MaxNumber(std::vector<int>& nums1,
    std::vector<int>& nums2, int len) {
  nums1.insert(nums1.begin(), 0);
  nums2.insert(nums2.begin(), 0);
  int m = nums1.size();
  int n = nums2.size();
  std::vector<std::vector<std::vector<int> > > dp(2,
    std::vector<std::vector<int> >(m, std::vector<int>(n, 0)));
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < m; j++) {
      for (int k = 0; k < n; k++) {
        if (j + k < i + 1) continue;
        if (i == 0) {
          if (j == 0) dp[i % 2][j][k] = std::max(nums2[k], dp[i % 2][j][k - 1]);
          else if (k == 0) dp[i % 2][j][k] = std::max(nums1[j], dp[i % 2][j - 1][k]);
          else {
            dp[i % 2][j][k] = std::max(nums1[j], nums2[k]);
            dp[i % 2][j][k] = std::max(dp[i % 2][j][k], dp[i % 2][j - 1][k]);
            dp[i % 2][j][k] = std::max(dp[i % 2][j][k], dp[i % 2][j][k - 1]);
          }
        } else {
          if (j == 0) {
            dp[i % 2][j][k] = dp[(i + 1) % 2][j][k - 1] * 10 + nums2[k];
            if (k - 1 >= i + 1) dp[i % 2][j][k] = std::max(dp[i % 2][j][k], dp[i % 2][j][k - 1]);
          }
          else if (k == 0) {
            dp[i % 2][j][k] = dp[(i + 1) % 2][j - 1][k] * 10 + nums1[j];
            if (j - 1 >= i + 1) dp[i % 2][j][k] = std::max(dp[i % 2][j][k], dp[i % 2][j - 1][k]);
          }
          else {
            int cur = std::max(dp[i % 2][j - 1][k], dp[i % 2][j][k - 1]);
            cur = std::max(dp[(i + 1) % 2][j][k - 1] * 10 + nums2[k], cur);
            cur = std::max(dp[(i + 1) % 2][j - 1][k] * 10 + nums1[j], cur);
            dp[i % 2][j][k] = cur;
          }
        }
        // LOG(INFO) << "[" << i % 2 << "][" << j << "][" << k << "]=" << dp[i % 2][j][k];
      }
    }
  }
  int max = INT_MIN;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (i + j < len) continue;
      max = std::max(max, dp[(len - 1) % 2][i][j]);
    }
  }
  std::vector<int> ans;
  while (max != 0) {
    ans.push_back(max % 10);
    max /= 10;
  }
  std::reverse(ans.begin(), ans.end());
  return ans;
}
}  // namespace algorithm

namespace NB {

std::vector<int> MaxNumber(std::vector<int>& nums, int len) {
  std::vector<int> ans;
  if (len <= 0) return ans;
  int drop = nums.size() - len;

  for (int i = 0; i < nums.size(); i++) {
    while (ans.size() && drop > 0 && ans.back() < nums[i]) {
      ans.pop_back();
      drop--;
    }
    ans.push_back(nums[i]);
  }
  ans.resize(len);
  return ans;
}

std::vector<int> Merge(std::vector<int> & nums1, std::vector<int> & nums2) {
  std::vector<int> ans;
  while (nums1.size() + nums2.size() > 0) {
    std::vector<int> & now = nums1 > nums2 ? nums1 : nums2;
    ans.push_back(now.front());
    now.erase(now.begin());
  }
  return ans;
}

std::vector<int> MaxNumber(std::vector<int>& nums1,
    std::vector<int>& nums2, int len) {
  int m = nums1.size();
  int n = nums2.size();
  std::vector<int> ans(len, 0);
  for (int i = std::max(0, len - n); i <= std::min(m, len); i++) {
    int rest = len - i;
    std::vector<int> vec1 = MaxNumber(nums1, i);
    std::vector<int> vec2 = MaxNumber(nums2, rest);
    ans = std::max(ans, Merge(vec1, vec2));
  }
  return ans;
}
}  // namespace NB


using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> num1;
  std::vector<int> num2;
  num1.push_back(3);
  num1.push_back(4);
  num1.push_back(6);
  num1.push_back(5);

  num2.push_back(9);
  num2.push_back(1);
  num2.push_back(2);
  num2.push_back(5);
  num2.push_back(8);
  num2.push_back(3);

  LOG(INFO) << JoinVector(NB::MaxNumber(num1, num2, 5));
  LOG(INFO) << JoinVector(MaxNumber(num1, num2, 5));
  num1.clear();
  num2.clear();
  num1.push_back(3);
  num1.push_back(9);

  num2.push_back(8);
  num2.push_back(9);
  LOG(INFO) << JoinVector(NB::MaxNumber(num1, num2, 3));
  LOG(INFO) << JoinVector(MaxNumber(num1, num2, 3));

  num1.clear();
  num2.clear();
  num1.push_back(6);
  num1.push_back(7);

  num2.push_back(6);
  num2.push_back(0);
  num2.push_back(4);
  LOG(INFO) << JoinVector(NB::MaxNumber(num1, num2, 5));
  LOG(INFO) << JoinVector(MaxNumber(num1, num2, 5));

  return 0;
}
