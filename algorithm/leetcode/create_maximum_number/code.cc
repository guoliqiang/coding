// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-25 15:29:32
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm2 {
std::string MaxNumber(const std::vector<int> & nums, int k) {
  int n = nums.size();
  std::vector<std::vector<std::string> > dp(2,
      std::vector<std::string>(n, std::string()));
  for (int i = 0; i < k; i++) {
    for (int j = i; j < n; j++) {
      if (i == 0) {
        dp[i % 2][j].push_back(nums[j] + '0');
      } else {
        dp[i % 2][j].clear();
        for (int k = i - 1; k < j; k++) {
          std::string cur = dp[(i + 1) % 2][k];
          cur.push_back(nums[j] + '0');
          dp[i % 2][j] = std::max(dp[i % 2][j], cur);
        }
      }
    }
  }
  std::string max;
  for (int i = 0; i < n; i++) {
    max = std::max(max, dp[(k - 1) % 2][i]);
  }
  return max;
}

std::vector<int> MaxNumber(std::vector<int>& nums1,
                           std::vector<int>& nums2,
                           int k) {
  std::vector<int> ans;
  std::string max1 = MaxNumber(nums1, k);
  std::string max2 = MaxNumber(nums2, k);

  std::string max = std::max(max1, max2);
  for (int i = 0; i < max.size(); i++) {
    ans.push_back(max[i] - '0');
  }
  return ans;
}

}  // namespace algorithm2

namespace algorithm {

std::vector<int> MaxNumber(std::vector<int>& nums1,
                           std::vector<int>& nums2,
                           int k) {
  nums1.insert(nums1.begin(), 0);
  nums2.insert(nums2.begin(), 0);

  int n1 = nums1.size();
  int n2 = nums2.size();

  std::vector<std::vector<std::vector<std::string> > > dp(3,
    std::vector<std::vector<std::string> >(n1,
    std::vector<std::string>(n2, std::string(""))));

  for (int i = 0; i < k; i++) {
    for (int j = 0; j < n1; j++) {
      for (int m = 0; m < n2; m++) {
        dp[i % 2][j][m].clear();
        if (j + m + 2 < i + 1) continue;

        if (i == 0 ) {
          dp[i % 3][j][m].push_back(std::max(nums1[j], nums2[m]) + '0');
        } else if (i == 1) {
          std::string cur;
          cur.push_back(std::max(nums1[j], nums2[m]) + '0');
          cur.push_back(std::min(nums1[j], nums2[m]) + '0');
          dp[i % 2][j][m] = cur;
        } else {
          for (int k1 = 0; k1 < j; k1++) {
            for (int k2 = 0; k2 < m; k2++) {
              if (k1 + k2 + 2 < i) continue;
              std::string cur = dp[(i - 1) % 3][k1][k2];
              cur.push_back(std::max(nums1[j], nums2[m]) + '0');
              dp[i % 3][j][m] = std::max(dp[i % 3][j][m], cur);
            }
          }
          for (int k1 = 0; k1 < j; k1++) {
            for (int k2 = 0; k2 < m; k2++) {
              if (k1 + k2 + 2 < i - 1) continue;
              std::string cur = dp[(i - 2) % 3][k1][k2];
              cur.push_back(std::max(nums1[j], nums2[m]) + '0');
              cur.push_back(std::min(nums1[j], nums2[m]) + '0');
              dp[i % 3][j][m] = std::max(dp[i % 3][j][m], cur);
            }
          }
        }
        LOG(INFO) << "size=" << i + 1 << "[" << j << "][" << m << "]=" << dp[i % 3][j][m];
      }
    }
  }

  std::string max;
  for (int i = 0; i < n1; i++) {
    for (int j = 0; j < n2; j++) {
    if (i + j + 2 < k) continue;
      max = std::max(max, dp[(k - 1) % 3][i][j]);
    }
  }

  std::vector<int> ans;
  for (int i = 0; i < max.size(); i++) {
    ans.push_back(max[i] - '0');
  }
  return ans;
}
}  // namespace algorithm


using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> num1;
  std::vector<int> num2;
  num1.push_back(3);
  num1.push_back(9);

  num2.push_back(8);
  num2.push_back(9);
  LOG(INFO) << JoinVector(MaxNumber(num1, num2, 3));
  return 0;

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

  LOG(INFO) << JoinVector(MaxNumber(num1, num2, 5));
  num1.clear();
  num2.clear();
  num1.push_back(6);
  num1.push_back(7);

  num2.push_back(6);
  num2.push_back(0);
  num2.push_back(4);
  LOG(INFO) << JoinVector(MaxNumber(num1, num2, 5));
  num1.clear();
  num2.clear();
  num1.push_back(3);
  num1.push_back(9);

  num2.push_back(8);
  num2.push_back(9);
  LOG(INFO) << JoinVector(MaxNumber(num1, num2, 3));

  return 0;
}
