// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-11 18:26:04
// File  : code.cc
// Brief :

/*
 * Accepted 180K  47MS  C++
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 105;
int N = 0;
int matrix[MAXN][MAXN];
int stack[MAXN];
int dfsn[MAXN];
int low[MAXN];
int id[MAXN];
int valid[MAXN];

int in[MAXN];
int out[MAXN];

int tail = 0;
int num = 0;
int tag = 0;

void Tarjan(int k) {
  dfsn[k] = low[k] = num++;
  stack[tail++] = k;
  int min = N;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (dfsn[i] == -1) Tarjan(i);
      if (low[i] < min) min = low[i];
    }
  }
  // LOG(INFO) << "min:" << min << " low[" << k << "]:" << low[k];
  if (min < low[k]) {
    low[k] = min;
  } else {
    tail--;
    while (stack[tail] != k) {
      id[stack[tail]] = tag;
      // LOG(INFO) << stack[tail] << ":" << tag;
      low[stack[tail]] = N;
      tail--;
    }
    id[stack[tail]] = tag++;
    low[stack[tail]] = N;
  }
}

void Base() {
  memset(dfsn, -1, sizeof(dfsn));
  memset(stack, 0, sizeof(stack));
  memset(id, -1, sizeof(id));
  memset(low, -1, sizeof(low));
  tag = 0;
  num = 0;
  tail = 0;
  for (int i = 0; i < N; i++) {
    if (dfsn[i] == - 1) Tarjan(i);
  }
  memset(valid, 0, sizeof(in));
  memset(valid, 0, sizeof(out));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0 && id[i] != id[j]) {
        in[id[j]]++;
        out[id[i]]++;
      }
    }
  }

  int i_num = 0;
  int o_num = 0;
  for (int i = 0; i < tag; i++) {
    if (in[i] == 0) i_num++;
    if (out[i] == 0) o_num++;
  }
  if (tag == 1) {
    printf("1\n0\n");
  } else {
    printf("%d\n%d\n", i_num, std::max(i_num, o_num));
  }
}

void Read() {
  int n = 0;
  scanf("%d", &n);
  getchar();
  N = n;
  memset(matrix, 0, sizeof(matrix));
  for (int i = 0; i < n; i++) {
    int t = 0;
    while (scanf("%d", &t) && t != 0) {
      t -= 1;
      matrix[i][t] = 1;
    }
    getchar();
  }
  Base();
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
