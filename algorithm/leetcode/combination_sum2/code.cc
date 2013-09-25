// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 20:00:06
// File  : code.cc
// Brief :
/*
 * 0 - 1 背包问题
 *
 *
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 22/22 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 80 milli secs
 * Progress: 172/172 test cases passed.
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
        dp[j][i] = std::max(dp[j][i - 1], candi[i] + dp[j - candi[i]][i - 1]);
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
    std::vector<int> bar = path;
    std::sort(bar.begin(), bar.end());
    rs.insert(bar);
  } else {
    if (dp[j][i - 1] == dp[j][i]) {
      Trace(candi, dp, path, i - 1, dp[j][i], rs);
    }
    if (j >= candi[i] && dp[j - candi[i]][i - 1] + candi[i] == dp[j][i]) {
      path.push_back(candi[i]);
      Trace(candi, dp, path, i - 1, dp[j][i] - candi[i], rs);
      path.pop_back();
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


int main(int argc, char** argv) {
  std::vector<int> candi;
  candi.push_back(10);
  candi.push_back(1);
  candi.push_back(2);
  candi.push_back(7);
  candi.push_back(6);
  candi.push_back(1);
  candi.push_back(5);
  int n = 8;
  std::vector<std::vector<int> > rs = CombinationSum(candi, n);
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << JoinVector(rs[i]);
  }
  return 0;
}
