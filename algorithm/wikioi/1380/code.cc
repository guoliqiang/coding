// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-18 12:05:45
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 6008;
int N;
int data[MAXN];
int degree[MAXN];
std::vector<int> ch[MAXN];
int dp[MAXN][2];

void Dp(int r) {
  if (ch[r].size() == 0) {
    dp[r][0] = 0;
    dp[r][1] = data[r];
  } else {
    int foo = 0;
    int bar = 0;
    for (int i = 0; i < ch[r].size(); i++) {
      Dp(ch[r][i]);
      foo += dp[ch[r][i]][0];  // No
      bar += std::max(dp[ch[r][i]][0], dp[ch[r][i]][1]);
    }
    dp[r][0] = bar;
    dp[r][1] = data[r] + foo;
  }
}

int Root() {
  memset(degree, 0, sizeof(degree));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < ch[i].size(); j++) degree[ch[i][j]]++;
  }
  int rs = -1;
  for (int i = 0; i < N; i++) {
    if (degree[i] == 0) rs = i;
  }
  return rs;
}

int Dp() {
  memset(dp, 0, sizeof(dp));
  int r = Root();
  Dp(r);
  return std::max(dp[r][0], dp[r][1]);
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    for (int i = 0; i < MAXN; i++) ch[i].clear();
    memset(data, 0, sizeof(data));
    for (int i = 0; i < N; i++) {
      int t;
      scanf("%d", &t);
      getchar();
      data[i] = t;
    }
    for (int i = 0; i < N - 1; i++) {
      int t1, t2;
      scanf("%d%d", &t1, &t2);
      getchar();
      ch[t2 - 1].push_back(t1 - 1);
    }
    int x, y;
    scanf("%d%d", &x, &y); // 0, 0
    getchar();

    int t = Dp();
    printf("%d\n", t);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
