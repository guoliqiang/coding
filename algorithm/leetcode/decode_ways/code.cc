// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 14:55:20
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 27/27 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 20 milli secs
 * Progress: 255/255 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

bool Bigger26OrSmall10(std::string & s, int i) {
  if (s[i - 1] > '2' || s[i - 1] == '0') return true;
  if (s[i - 1] == '2' && s[i] > '6') return true;
  return false;
}

void DP(std::string & s, std::vector<int> & dp) {
  if (s[0] <= '0') dp[0] = 0;
  else dp[0] = 1;

  if (dp.size() == 1) return;
  if (s[1] <= '0') dp[1] = 0;
  else dp[1] = dp[0];

  if (!Bigger26OrSmall10(s, 1)) {
    dp[1] += 1;
  }
  
  for (int i = 2; i < dp.size(); i++) {
    if (s[i] <= '0') dp[i] = 0;
    else dp[i] = dp[i - 1];
    if (!Bigger26OrSmall10(s, i)) dp[i] += dp[i - 2];
  }
}

int NumDecodings(std::string & s) {
  if (s.size() == 0) return 0;
  std::vector<int> dp(s.size(), 0);
  DP(s, dp);
  return dp[s.size() - 1];
}

}  // namespace algorithm

namespace clear {
bool Valid(char x, char y) {
    if (x == '0') return false;
    int cur = (x - '0') * 10 + (y - '0');
    return cur >= 1 && cur <= 26;
}

int NumDecodings(std::string s) {
    int n = s.size();
    if (n == 0) return 0;
    if (s[0] == '0') return 0;

    std::vector<int> dp(n, 0);

    for (int i = 0; i < n; i++) {
        if (i == 0) dp[i] = 1;
        else if (i == 1) {
            if (s[i] == '0') {
                if (!Valid(s[i - 1], s[i])) return 0;
                else dp[i] = 1;
            } else dp[i] = dp[i - 1] + (Valid(s[i - 1], s[i]) ? 1 : 0);
        } else {
            if (s[i] == '0') {
                if (!Valid(s[i - 1], s[i])) return 0;
                dp[i] = dp[i - 2];
            } else dp[i] = dp[i - 1] + (Valid(s[i - 1], s[i]) ? dp[i - 2] : 0);
        }
    }
    return dp[n - 1];
}
}  // namespace clear

using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "01";
  LOG(INFO) << NumDecodings(str);
  return 0;
}
