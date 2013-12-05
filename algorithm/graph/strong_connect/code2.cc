// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-24 20:16:48
// File  : code2.cc
// Brief :

// tarjan强联通分量
#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int dfn[MAX];
int low[MAX] = {0};
int id[MAX] = {0};
int stack[MAX] = {0};
int stop = 0;

int component_tag = 0;
int cnt = 1;


void Tarjan(int k) {
  dfn[k] = low[k] = cnt++;
  stack[stop++] = k;
  int min = low[k];
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (dfn[i] == -1) Tarjan(i);
      min = std::min(min, low[i]);
    }
  }
  if (low[k] > min) {
    low[k] = min;
  } else {
    LOG(INFO) << "strong component " << component_tag << ":";
    stop--;
    while (stack[stop] != k) {
      LOG(INFO) << stack[stop];
      id[stack[stop]] = component_tag;
      low[stack[stop]] = N;  // trick
      stop--;
    }
    LOG(INFO) << k;
    id[k] = component_tag;
    low[k] = N;
    component_tag++;
  }
}

void Read(std::vector<std::vector<int> > & v) {
  N = v.size();
  for (int i = 0; i < N; i++) dfn[i] = -1;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) matrix[i][j] = v[i][j];
  }
}

void Find() {
  for (int i = 0; i < N; i++) {
    if (dfn[i] == -1) Tarjan(i);
  }
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 6;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][2] = 1;
  v[0][1] = 1;
  v[1][3] = 1;
  v[2][3] = 1;
  v[3][0] = 1;
  v[2][4] = 1;
  v[3][5] = 1;
  v[4][5] = 1;
  Read(v);
  Find();
  return 0;
}
