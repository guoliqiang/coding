// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-09 18:49:44
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1005;
int dp[MAXN][MAXN];
char data[MAXN];

int Dp() {
 int n = strlen(data);
 memset(dp, 0, sizeof(dp));
 for (int k = 0; k < n; k++) {
   for (int i = 0; i < n- k; i++) {
     if (k == 0) dp[i][i + k] = 0;
     else if (k == 1) dp[i][i + k] = data[i] == data[i + k] ? 0 : 1;
     else {
       int t = std::min(dp[i + 1][i + k], dp[i][i + k - 1]) + 1;
       if (data[i] == data[i + k]) dp[i][i + k] = std::min(dp[i + 1][i + k - 1], t);
       else dp[i][i + k] = t;
     }
   }
 }
 return dp[0][n - 1];
}

void Read() {
  while (scanf("%s", data) != EOF) {
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
