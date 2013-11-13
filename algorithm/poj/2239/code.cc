// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-12 17:38:43
// File  : code.cc
// Brief :

/*
 * Accepted 512K  32MS
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 310;
const int MAXM = 7 * 12;
int N = 0;
int matrix[MAXN][MAXN];
int link[MAXN];
int link_l[MAXN];
// 保存结果
int visited[MAXN];

bool Search(int k) {
  for (int i = 0; i < MAXM; i++) {
    if (matrix[k][i] > 0 && visited[i] == 0) {
      visited[i] = 1;
      if (link[i] == -1 || Search(link[i])) {
        link[i] = k;
        link_l[k] = i;
        return true;
      }
    }
  }
  return false;
}

void MaxMatch() {
  memset(link, -1, sizeof(link));
  memset(link_l, -1, sizeof(link_l));
  int rs = 0;
  for (int i = 0; i < N; i++) {
    memset(visited, 0, sizeof(visited));  // 保证不能重复找
    if (Search(i)) rs++;
  }
  printf("%d\n", rs);
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF) {
    memset(matrix, 0, sizeof(matrix));
    getchar();
    N = n;
    for (int k = 0; k < n; k++) {
      int t = 0;
      scanf("%d", &t);
      for (int i = 0; i < t; i++) {
        int p, q;
        scanf("%d%d", &p, &q);
        matrix[k][(p - 1) * 12 + (q - 1)] = 1;
      }
      getchar();
    }
    MaxMatch();
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
