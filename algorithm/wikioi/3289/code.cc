// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-20 17:33:15
// File  : code.cc
// Brief :

// http://www.wikioi.com/solution/list/3289/
// f[i][0]表示从1-i且最后一段为上升的最大保留花数目
// f[i][1]表示从1-i且最后一段为下降的最大保留花数目
// 那么当a[i]大于a[i-1]时 前i个最后为上升的可以是前i-1下降加上第i个的长度或前i-1上升的长度的最大值
// a[i]小于a[i-1]时 情况相反也类似
// 注意a[i]==a[i-1]时，f[i][0]=f[i-1][0]; f[i][1]=f[i-1][1];
// 其实本题考虑到f[i]只跟f[i-1]有关
// O(n)的dp
//
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 100003;
int N;
int v[MAXN];
int dp1[MAXN], dp2[MAXN];

int Dp() {
  if (N == 0) return 0;
  memset(dp1, 0, sizeof(dp1));
  memset(dp2, 0, sizeof(dp2));
  dp1[1] = 1;
  dp2[1] = 1;
  int rs = 1;
  for (int i = 2; i <= N; i++) {
    dp1[i] = 1;
    dp2[i] = 1;
    bool f1 = false;
    bool f2 = false;
    bool f3 = false;
    bool f4 = false;
    for (int j = i - 1; j >= 0; j--) {
      if (v[j] > v[i]) {
        if (dp1[j] % 2 == 0) {
          f1 = true;
          dp1[i] = std::max(dp1[i], dp1[j] + 1);
        }
        if (dp2[j] % 2 == 1) {
          f2 = true;
          dp2[i] = std::max(dp2[i], dp2[j] + 1);
        }
      } else if (v[j] < v[i]) {
        if (dp1[j] % 2 == 1) {
          f3 = true;
          dp1[i] = std::max(dp1[i], dp1[j] + 1);
        }
        if (dp2[j] % 2 == 0) {
          f4 = true;
          dp2[i] = std::max(dp2[i], dp2[j] + 1);
        }
      }
      if (f1 && f2 && f3 && f4) break;
    }
    rs = std::max(dp1[i], rs);
    rs = std::max(dp2[i], rs);
  }
  return rs;
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    for (int i = 0; i < N; i++) {
      int t = 0;
      scanf("%d", &t);
      v[i + 1] = t;
    }
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
