// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-30 10:24:59
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 100;
int N;
int matrix[MAXN][MAXN];
int vis[MAXN];

void DFS(int k, int t, std::vector<int> & path) {
  vis[k] = 1;
  if (k == t) {
    LOG(INFO) << JoinVector(path);
  } else {
    for (int i = 0; i < N; i++) {
      if (vis[i] == 0) {
        path.push_back(i);
        DFS(i, t, path);
        path.pop_back();
      }
    }
  }
  vis[k] = 0;
}

void DFS(int s, int t) {
  std::vector<int> path;
  memset(vis, 0, sizeof(vis));
  DFS(s, t, path);
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
