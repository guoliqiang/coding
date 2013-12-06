// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-04 22:45:21
// File  : code.cc
// Brief :

/*
 * 最长公共递增子序列（不是子串）
 * hdu1423
 *
 * 子序列：可以不连续
 * 子串：必须连续
 *
 * */
#include "base/public/common_head.h"

namespace algorithm {
/*
 * 每一轮的dp[j]
 * 表示当前的str1[0~i] 与 str2[0~j]中以str2[j]结尾的递增子序列的长度，并不一定以str1[i]结尾
 * 在str1[i] != str[j]时：
 *   dp[j] 保存的是之前的计算结果的最大值，其含义也就是:
 *         str1[0~i] 与 str2[0~j]中以str2[j]结尾的递增子序列的长度,其一定不以str1[i]结尾.
 *
 *
 *  O(n^2)的实现
 *
 * */

/*
 *  容易理解的版本
 * */

void DP(std::string & str1, std::string & str2) {
  int m = str1.size();
  int n = str2.size();
  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  std::vector<std::vector<std::pair<int, int> > > path(m, std::vector<std::pair<int, int> >(n, std::make_pair(0, 0)));
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (str1[i] == str2[j]) {
        int t = 0;
        int index = -1;
        for (int k = 0; i > 0 && k < j; k++) {
          if (str2[k] < str2[j]) {
            if (t < dp[i - 1][k]) {
              t = dp[i - 1][k];
              index = k;
            }
          }
        }
        dp[i][j] = t + 1;
        path[i][j] = std::make_pair(i, index);
      } else {
        dp[i][j] = dp[i - 1][j];
        path[i][j] = path[i - 1][j];
      }
    }
  }
}

// zoj 2432
void DP(std::string & str1, std::string & str2, std::vector<int> & dp,
        std::vector<std::vector<int> > & path) {
  int m = str1.size();
  int n = str2.size();
  dp.resize(n, 0);
  path.resize(m, std::vector<int>(n, 0));
  for (int i = 0; i < m; i++) {
    int k = -1;
    if (i > 0) {
      for (int k = 0; k < n; k++) path[i][k] = path[i - 1][k];
    }
    for (int j = 0; j < n; j++) {
      if (str2[j] == str1[i] && (k == -1 || dp[k] + 1 > dp[j])) {
        dp[j] = k == -1 ? 1 : dp[k] + 1;
        if (k != -1) path[i][j] = (i + 1) * n + k;
      }
      if (str2[j] < str1[i] && (k == -1 || dp[j] > dp[k])) k = j;
    }
  }
}
// zoj 2432代码 的trace不使用size
void Trace(int k, int x, std::string & str2,
           std::vector<std::vector<int> > & path,
           int size) {
  if (size == 0) return;
  LOG(INFO) << "\"" << str2[k] << "\"";
  int t = path[x][k];
  k = t % str2.size();
  x = t / str2.size() - 1;
  Trace(k, x, str2, path, size - 1);
}

int LSIC(std::string & str1, std::string & str2) {
  std::vector<int> dp;
  std::vector<std::vector<int> > path;
  DP(str1, str2, dp, path);
  int index = -1;
  int max = -1;
  for (int i = 0; i < dp.size(); i++) {
    if (dp[i] > max) {
      max = dp[i];
      index = i;
    }
  }
  Trace(index, str1.size() - 1, str2, path, max);
  return max;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str1 = "14250";
  std::string str2 = "0124";
  LOG(INFO) << LSIC(str1, str2);
  return 0;
}
