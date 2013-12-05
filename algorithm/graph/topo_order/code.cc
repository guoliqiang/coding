// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-24 00:54:04
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int count[MAX] = {0};
int matrix[MAX][MAX] = {{0}};
int queue[MAX] = {0};

bool TopoOrder() {
  memset(count, 0, sizeof(count));
  memset(queue, 0, sizeof(queue));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0) count[j]++;
    }
  }
  int tail = 0;
  for (int i = 0; i < N; i++) {
    if (count[i] == 0) {
      queue[tail++] = i;
    }
  }
  int k = 0;
  while (k < tail) {
    int t = queue[k++];
    for (int i = 0; i < N; i++) {
      if (matrix[t][i] > 0) count[i]--;
      if (count[i] == 0) queue[tail++] = i;
    }
  }
  return tail == N ? true : false;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
