// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-18 03:50:55
// File  : code.cc
// Brief :

/*
 * 使用STL的stack会超时
 * Accepted  148k  1579MS
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 2010;
int N = 0;
int vec[MAXN];
int stack[MAXN];
int MaxVec() {
  memset(stack, 0, sizeof(stack));
  int top = 0;  // 指向第一个可用位置
  int max = 0;
  for (int i = 0; i < N + 1; i++) {
    if (top == 0 || vec[i] >= vec[stack[top - 1]]) {
      stack[top++] = i;;
    } else {
      int t = stack[top - 1];
      top--;
      int foo = (top == 0) ? vec[t] * i : vec[t] * (i - stack[top - 1] - 1);
      max = std::max(max, foo);
      i--;
    }
  }
  return max;
}

void Read() {
  int m, n;
  while (scanf("%d%d", &m, &n) != EOF) {
    N = n;
    getchar();
    int max = 0;
    memset(vec, 0, sizeof(vec));
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        int t = 0;
        scanf("%d", &t);
        if (t == 1) vec[j] += 1;
        else vec[j] = 0;
      }
      getchar();
      max = std::max(max, MaxVec());
    }
    printf("%d\n", max);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
