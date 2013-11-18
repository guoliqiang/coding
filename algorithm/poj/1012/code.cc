// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-16 04:52:56
// File  : code.cc
// Brief :

/*
 * Accepted 132K  47MS  C++
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 14;
int result[MAXN];
int N = 14;

bool Ok(int k, int m) {
  int n = k << 1;
  int j = 0;
  for (int i = 0; i < k; i++) {
    j = (j + m -1) % (n - i); // 第i次删除要删除的数在当前串中排在第几位
    if(j < k) return false;
  }
  return true;
}

/*
 * 考虑最后剩下k+2个元素时
 * GGGG...GB1B2
 * 如果当前需要删除的是B1,则m一定是k+2，这样最有一轮才有可能删除B2
 *
 * 如果当前需要删除的是B2，则m一定等于k+1，这样最有一轮才有可能删除B2
 *
 * 即 k%(k + 1) == 0 或 1
 * */

void Jose() {
  memset(result, 0, sizeof(result));
  for (int k = 1; k < 14; k++) {
    int m = k + 1;
    while (true) {
      if (Ok(k, m)) {
        result[k] = m;
        break;
      }
      if (Ok(k, m + 1)) {
        result[k] = m + 1;
        break;
      }
      m += (k + 1);
    }
  }
}

void Read() {
  Jose();
  int k = 0;
  while (scanf("%d", &k) != EOF && k) {
    printf("%d\n", result[k]);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
