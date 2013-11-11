// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-10 18:01:22
// File  : code.cc
// Brief :

/*
 * Accepted 2720K 266MS
 *
 * */

/*
 * 用0表示没有放木块，用1表示放了木块。此外，对于一个横放的木块，
 * 对应的两位都用1表示；对于一个竖放的木块，第一行用1表示，第二行用0表示。
 * 想象成一个木板只占一个方格（即把竖直放的木板的下半截扣掉）
 * http://www.cppblog.com/sdfond/archive/2009/07/31/91761.html
 * 
 * */

/*
 * 对于2 * 2的，方法如下：
 * 11
 * 00
 *
 * 11
 * 11
 *
 * */


#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 11;
int N = 0;
const int MAXM = 11;
int M = 0;

std::vector<int> state[1 << MAXN];
long long dp[MAXM + 2][1 << MAXN];  // int can not be accepted

void State(int s1, int s2, int k) {
  if (k >= N) {
    if (s1 < (1 << N) && s2 < (1 << N)) {
      state[s1].push_back(s2);
    }
  } else {
    // 对于上一行到下一行的转移，规定上一行一定填满
    
    // 上面是1下面是0 
    State((s1 << 1 | 0), (s2 << 1 | 1), k + 1);
    // 上面是0(竖放的下半截),下面是1（肯定不能是0）
    State((s1 << 1 | 1), (s2 << 1 | 0), k + 1);
    // 上面横放，下面横放
    State((s1 << 2 | 3), (s2 << 2 | 3), k + 2);
  }
}

void State() {
  State(0, 0, 0);
}

long long DP() {
  memset(state, 0, sizeof(state));
  memset(dp, 0, sizeof(dp));
  State();
  dp[0][0] = 1;  // 封顶
  for (int i = 1; i < M + 2; i++) {
    for (int j = 0; j < (1 << N); j++) {
      std::vector<int> & t = state[j];
      for (std::vector<int>::iterator k = t.begin(); k != t.end(); k++) {
        dp[i][*k] += dp[i - 1][j];
      }
    }
  }
  return dp[M + 1][(1 << N) - 1];  // 封底
}

void Read() {
  int n = 0;
  int m = 0;
  while (scanf("%d%d", &m, &n) != EOF && (m || n)) {
    M = m;
    N = n;
    for (int i = 0; i < (1 << MAXN); i++) state[i].clear();
    printf("%lld\n", DP());
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
