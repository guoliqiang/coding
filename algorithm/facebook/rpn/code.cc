// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-03 17:33:30
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int Dp(std::string str) {
  LOG(INFO) << str;
  int n = str.size();
  std::vector<int> dp(n, 0);
  dp[0] = str[0] == 'x' ? 0 : 1;
  LOG(INFO) << "dp[" << 0 << "]:" << dp[0];
  for (int i = 1; i < n; i++) {
    dp[i] = dp[i - 1] + 1;
    int r = str[i] == '*' ? 1 : 0;
    for (int j = 0; j < i; j++) {
      if (str[j] == '*') {
        if (j == 0) {
          dp[i] = std::min(1 + r + i - 1, dp[i]);
          // LOG(INFO) << "dp[" << i << "]:" << dp[i];
        } else {
          dp[i] = std::min(dp[j - 1] + r + i - j - 1, dp[i]);
          // LOG(INFO) << "dp[" << i << "]:" << dp[i];
        }
      } else {
        dp[i] = std::min(dp[j] + 1 + r + i - j - 1, dp[i]);
        if (j > 0) {
          dp[i] = std::min(dp[j - 1] + 1 + r + i - j - 1, dp[i]);
        }
      }
    }
    dp[i] = std::min(dp[i], r + i);
    LOG(INFO) << "dp[" << i << "]:" << dp[i];
  }
  return dp[n - 1];
}

int DpR(std::string str) {
  int n = str.size();
  std::vector<std::vector<int> > dp(n, std::vector<int>(n, 0));
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n - k; i++) {
      if (k == 0) dp[i][i + k] = str[i] == 'x' ? 0 : 1;
      else {
        bool star = str[i + k] == '*' ? 1 : 0;
        dp[i][i + k] = std::min(dp[i][i + k - 1] + 1, star + k);
        LOG(INFO) << "dp[" << i << "][" << i + k << "]:" << dp[i][i + k];
        for (int j = i; j < i + k; j++) {
          if (j == i + k - 1) {
            dp[i][i + k] = std::min(dp[i][i + k], dp[i][j] + 1 + !star);
            LOG(INFO) << "dp[" << i << "][" << i + k << "]:" << dp[i][i + k];
          } else {
            dp[i][i + k] = std::min(dp[i][i + k], dp[i][j] + dp[j + 1][i + k - 1] + !star);
            LOG(INFO) << "dp[" << i << "][" << i + k << "]:" << dp[i][i + k] << " j:" << j << " k:" << k;
          }
        }
      }
      LOG(INFO) << "dp[" << i << "][" << i + k << "]:" << dp[i][i + k];
    }
  }
  return dp[0][n - 1];
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  LOG(INFO) << DpR("xxx");
  return 0;
  LOG(INFO) << Dp("*xx");
  LOG(INFO) << Dp("x**x*xx");
  LOG(INFO) << Dp("***");
  LOG(INFO) << Dp("xxx");
  return 0;
}
