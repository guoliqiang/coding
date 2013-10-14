// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-13 20:37:00
// File  : code.cc
// Brief :
// http://www.cnblogs.com/kakamilan/archive/2012/07/10/2583903.html

#include "base/public/common_head.h"

namespace algorithm {
using namespace std;

static const int MAX = 200100;
std::vector<int> matrix[MAX];
int deep[MAX] = {-1};
int lowu[MAX] = {-1};
int father[MAX] = {-1};
int is_bridge[MAX] = {-1};
int dfn[MAX] = {-1};
int dfs_clock = 0;
int idx = 0;
int sum = 0;
int M = 0;
int N = 0;

// 注意是无向图
// deep数组将图转换成了有向树
void DFS(int deepth, int ifather, int cur) {
  father[cur] = ifather;
  deep[cur] = deepth;
  dfn[cur] = lowu[cur] = ++dfs_clock;
  for (int i = 0; i < matrix[cur].size(); i++) {
    int t = matrix[cur][i];
    if (t == ifather) continue;
    if (dfn[t] == -1) {
      DFS(deepth + 1, cur, t);
      lowu[cur] = std::min(lowu[t], lowu[cur]);
      if (lowu[t] > dfn[cur]) {
        sum++;
        is_bridge[t] = 1;
      }
    } else {
      // 因为有重边 重边算一条, 必须有 min
      lowu[cur] = std::min(lowu[cur], dfn[t]);
    }
  }
}


void FindBridge() {
  dfs_clock = 0;
  for (int i = 1; i <= N; i++) {
    father[i] = i;
    deep[i] = -1;
    lowu[i] = -1;
    dfn[i] = -1;
  }
  memset(is_bridge, 0, sizeof(is_bridge));
  DFS(0, -1, 1);
}

void LCA(int x, int y) {
  if (deep[x] > deep[y]) std::swap(x, y);
  if (deep[y] > deep[x]) {
    while (deep[y] != deep[x]) {
      if (is_bridge[y]) {
        is_bridge[y] = 0;
        sum--;
      }
      y = father[y];
    }
  }
  while (x != y) {
    // LOG(INFO) << x << " " << y;
    if (is_bridge[x]) {
      is_bridge[x] = 0;
      sum--;
    }
    if (is_bridge[y]) {
      is_bridge[y] = 0;
      sum--;
    }
    x = father[x];
    y = father[y];
  }
}

void ReadConsole() {
  freopen("test.txt", "r", stdin);
  int num = 1;
  while (scanf("%d %d", &N, &M) == 2 && (M + N)) {
    sum = 0;
    for (int i = 1; i <= N; i++) matrix[i].clear();
    for (int i = 0; i < M; i++) {
      int a = 0;
      int b = 0;
      scanf("%d %d", &a, &b);
      matrix[a].push_back(b);
      matrix[b].push_back(a);
    }
    printf("Case %d:\n", num++);
    FindBridge();
    // LOG(INFO) << "bridge:" << sum;
    int q = 0;
    scanf("%d", &q);
    for (int i = 0; i < q; i++) {
      int na = 0;
      int nb = 0;
      scanf("%d %d", &na, &nb);
      LCA(na, nb);
      printf("%d\n", sum);
    }
    printf("\n");
  }
}


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  ReadConsole();
  return 0;
}
