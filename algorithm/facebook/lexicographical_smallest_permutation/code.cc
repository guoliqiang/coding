// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-03 13:52:12
// File  : code.cc
// Brief :

// http://stackoverflow.com/questions/15817946/lexicographical-smallest-permutation-in-matrix-with-restricted-exchange

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 100;
int N;
int matrix[MAXN][MAXN];
int vis[MAXN];
int data[MAXN];

void DFS(int k, std::vector<int> & v) {
  vis[k] = 1;
  v.push_back(k);
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] == 1 && vis[i] == 0) DFS(i, v);
  }
}

void Solve() {
  memset(vis, 0, sizeof(vis));
  for (int i = 0; i < N; i++) {
    if (vis[i] == 0) {
      std::vector<int> v;
      DFS(i, v);
      std::vector<int> num;
      for (int i = 0; i < v.size(); i++) num.push_back(data[v[i]]);
      std::sort(v.begin(), v.end());
      std::sort(num.begin(), num.end());
      for (int i = 0; i < v.size(); i++) data[v[i]] = num[i];
    }
  }
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    for (int i = 0; i < N; i++) {
      scanf("%d", &data[i]);
    }
    getchar();
    memset(matrix, 0, sizeof(matrix));
    char ch[101];
    for (int i = 0; i < N; i++) {
      scanf("%s", ch);
      getchar();
      for (int j = 0; j < N; j++) {
        if (ch[j] == 'Y') matrix[i][j] = matrix[j][i] = 1;
      }
    }
    Solve();
    for (int i = 0; i < N; i++) printf("%d ", data[i]);
    printf("\n");
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
