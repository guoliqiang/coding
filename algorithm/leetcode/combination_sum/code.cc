// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 20:00:06
// File  : code.cc
// Brief :
/*
 * 0 - 1 背包问题
 *
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 17/17 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 100 milli secs
 * Progress: 168/168 test cases passed.
 *
 * */

#include <vector>
#include <set>
#include <iostream>
#include "base/public/logging.h"
#include "base/public/string_util.h"

namespace algorithm {
void DP(std::vector<int> & candi, int n, int **dp) {
  for (int i = 0; i < candi.size(); i++) dp[0][i] = 0;
  for (int i = 0; i <= n ; i++) dp[i][0] = 0;

  for (int i = 1; i < candi.size(); i++) {
    for (int j = 1; j <= n; j ++) {
      if (j < candi[i]) dp[j][i] = dp[j][i - 1];
      else {
        int max = 0;
        for (int k = 0; k <= (j / candi[i]); k++) {
          // LOG(INFO) << "max:" << max << " k * candi[i] + dp[i - 1][j - k * candi[i]] :"
          //           << k * candi[i] + dp[i - 1][j - k * candi[i]];
          // LOG(INFO) << "i: " << i << " j:" << j << " k:" << k << " k * candi[i]:"
          //           << k * candi[i] << " j - k * candi[i]:" << j - k * candi[i];
          // LOG(INFO) << "dp[i - 1][j - k * candi[i]] : "
          //           << dp[i - 1][j - k * candi[i]];
          max = std::max(max, k * candi[i] + dp[j - k * candi[i]][i - 1]);
        }
        dp[j][i] = max;
      }
      // LOG(INFO) << "dp[" << i << "][" << j << "]" << dp[i][j];
    }
  }
}

void Out(int ** dp, int n, int m) {
  for (int i = 0; i <= n; i++) {
    for (int j = 0; j < m; j++) {
      std::cout << dp[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void Trace(std::vector<int> & candi, int ** dp,
           std::vector<int> & path,
           int i, int j,
           std::set<std::vector<int> > & rs) {
  if (i == 0 || j == 0) {
    if (path.empty()) return;
    std::vector<int> bar = path;
    std::sort(bar.begin(), bar.end());
    rs.insert(bar);
  } else {
    for (int k = 0; k <= j / candi[i]; k++) {
      int foo = 0;
      for (int l = 0; l < k; l++) foo += candi[i];
      // LOG(INFO) << "k:" << k << " candi[" << i << "]:" << candi[i] << " foo:" << foo;
      if (dp[j - foo][i - 1] + foo != dp[j][i]) continue;
      // LOG(INFO) << "from dp[" << j << "][" << i << "]:" << dp[j][i];
      // LOG(INFO) << "search " << "dp[" << j - foo << "][" << i - 1 << "]:" << dp[j - foo][i - 1];
      for (int l = 0; l < k; l++) path.push_back(candi[i]);
      // LOG(INFO) << JoinVector(path);
      Trace(candi, dp, path, i - 1, dp[j][i] - foo, rs);
      // LOG(INFO) << JoinVector(path);
      for (int l = 0; l < k; l++) path.pop_back();
      // LOG(INFO) << JoinVector(path);
    }
  }
}

std::vector<std::vector<int> > CombinationSum(std::vector<int> candi, int n) {
  candi.push_back(-1);
  std::sort(candi.begin(), candi.end());
  int ** dp = new int * [n + 1];
  for (int i = 0; i <= n; i++) {
    dp[i] = new int[candi.size()];
  }
  DP(candi, n, dp);
  Out(dp, n, candi.size());
  std::vector<std::vector<int> > rs;
  std::set<std::vector<int> > tmp;
  std::vector<int> path;
  if (dp[n][candi.size() - 1] == n) Trace(candi, dp, path, candi.size() - 1, n, tmp);
  for (std::set<std::vector<int> >::iterator i = tmp.begin(); i != tmp.end(); i++) {
    rs.push_back(*i);
  }
  for (int i = 0; i <=n; i++) delete [] dp[i];
  delete [] dp;
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
void Combine(std::vector<int> & num, int target, int k, int cur, std::vector<int> path, std::set<std::vector<int> > &rs) {
  if (k == num.size() || cur == target) {
      if (cur == target) rs.insert(path);
  } else {
      while (cur <= target) {
          Combine(num, target, k + 1, cur, path, rs);
          cur += num[k];
          path.push_back(num[k]);
      }
  }
}
class Solution {
public:
    vector<vector<int> > combinationSum(vector<int> &candidates, int target) {
        // Note: The Solution object is instantiated only once and is reused by each test case.
        std::sort(candidates.begin(), candidates.end());
        std::vector<int> path;
        std::set<std::vector<int> > tmp;
        Combine(candidates, target, 0, 0, path, tmp);
        return std::vector<std::vector<int> >(tmp.begin(), tmp.end());
    }
};
}  // namespace twice


int main(int argc, char** argv) {
  std::vector<int> candi;
  candi.push_back(7);
  candi.push_back(3);
  candi.push_back(2);
  // candi.push_back(7);
  int n = 18;
  std::vector<std::vector<int> > rs = CombinationSum(candi, n);
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << JoinVector(rs[i]);
  }
  return 0;
}
