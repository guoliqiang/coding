// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-10 21:31:58
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 305;
int N;
int cnt;
int left;
int right;

int vec[MAXN];

void Solve() {
  bool first = true;
  while (cnt) {
    int tl = N;
    int tr = 0;
    for (int i = std::max(left - 1, 0); i <= std::min(right + 1, N - 1) && cnt; i++) {
      if (vec[i] > 0) {
        if (!first && i == std::max(left - 1, 0)) {}
        else {
          printf("P");
          vec[i]--;
          cnt--;
        }
      }
      if (i != std::min(right + 1, N - 1)) printf("R");
      if (vec[i] > 0) tl = std::min(tl, i);
    }
    left = tl;
    for (int i = std::min(right + 1, N - 1); i >= std::max(left - 1, 0) && cnt; i--) {
      if (vec[i] > 0 && i != std::min(right + 1, N - 1)) {
        printf("P");
        vec[i]--;
        cnt--;
      }
      if (i != std::max(left - 1, 0)) printf("L");
      if (vec[i] > 0) tr = std::max(tr, i);
    }
    right = tr;
    first = false;
  }
  printf("\n");
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    left = 0;
    right = 0;
    cnt = 0;
    for (int i = 0; i < N; i++) {
      scanf("%d", &(vec[i]));
      cnt += vec[i];
      if (vec[i] > 0) right = i;
    }
    getchar();
    Solve();
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
