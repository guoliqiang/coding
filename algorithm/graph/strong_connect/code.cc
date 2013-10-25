// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-24 04:32:01
// File  : code.cc
// Brief :
// 无向图强联通分支

#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int low[MAX] = {0};
int dfn[MAX] = {0};
int st[MAX] = {0};
int id[MAX] = {0};
int sp = 0;
int tag = 0;
int cnt = 0;

void Search(int k) {
  st[sp++] = k;
  low[k] = dfn[k] = ++cnt;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (!dfn[i]) {
        Search(i);
        if (low[i] < low[k]) low[k] = low[i];
      } else {
        if (dfn[i] < dfn[k]) {
          int j = 0;
          while (j < sp && st[j] != i) j++;
          if (j < cnt && dfn[i] < low[k]) low[k] = dfn[i];
        }
      }
    }
  }
  LOG(INFO) << "low[" << k << "]:" << low[k] << " " << "dfn[" << k << "]:" << dfn[k];
  if (low[k] == dfn[k]) {
    tag++;
    LOG(INFO) << k;
    while (st[sp] != k) {
      sp--;
      id[st[sp]] = tag;
      LOG(INFO) << "id[" << st[sp] << "]:" << id[st[sp]];
    }
  }
}

int Find() {
  sp = 0;
  cnt = 0;
  memset(dfn, 0, sizeof(dfn));
  for (int i = 0; i < N; i++) {
    if (!dfn[i]) Search(i);
  }
  return tag;
}

void Read(std::vector<std::vector<int> > & v) {
  N = v.size();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 3;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][1] = 1;
  v[1][2] = 1;
  v[2][1] = 1;
  v[2][0] = 1;
  Read(v);
  LOG(INFO) << Find();
  return 0;
}
