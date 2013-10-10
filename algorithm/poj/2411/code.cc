// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-10 17:43:01
// File  : code.cc
// Brief :

/*
 * 用0表示没有放木块，用1表示放了木块。此外，对于一个横放的木块，
 * 对应的两位都用1表示；对于一个竖放的木块，第一行用1表示，第二行用0表示。
 * http://www.cppblog.com/sdfond/archive/2009/07/31/91761.html
 * the code in before url can be accepted.
 *
 * This is wrong answer by poj, but just now i can not find the bug.
 * 
 * */

#include "base/public/common_head.h"


namespace algorithm {

void GetValidDFS(int s1, int s2, int col, int n, std::map<int, std::set<int> > & state) {
  if (col >= n) {
    if (s1 < (1 << n) && s2 < (1 << n)) {
      if (!state.count(s1)) state.insert(std::make_pair(s1, std::set<int>()));
      state[s1].insert(s2);
    }
  } else {
    GetValidDFS((s1 << 1) | 1, s2 << 1, col + 1, n, state);
    GetValidDFS(s1 << 1, (s2 << 1) | 1, col + 1, n, state);
    GetValidDFS((s1 << 2) | 3, (s2 << 2) | 3, col + 2, n, state);
  }
}

int DP(int m, int n) {
  std::map<int, std::set<int> > state;
  GetValidDFS(0, 0, 0, n, state);
  std::vector<std::vector<int> > dp(m + 2, std::vector<int>(1 << n, 0));
 
  dp[0][0] = 1;  // 封顶
  for (int i = 1; i <= m + 1; i++) {
    for (int j = 0; j < (1 << n); j++) {
      if (!state.count(j)) continue;
      for (std::set<int>::iterator k = state[j].begin(); k != state[j].end(); k++) {
        dp[i][j] += dp[i - 1][*k];
      }
    }
  }
  int t = dp[m + 1][(1 << n) - 1];  // 封底
  return t;
}

void ReadFromConsole() {
  int m = 0;
  int n = 0;
  while (std::cin >> m >> n) {
    if (m == 0 && n == 0) return;
    std::cout << DP(m, n) << std::endl;
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << DP(2, 3);
  LOG(INFO) << DP(4, 11);
  ReadFromConsole();
  return 0;
}
