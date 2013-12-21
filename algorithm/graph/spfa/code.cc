// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-21 11:36:18
// File  : code.cc
// Brief :
// http://www.nocow.cn/index.php/SPFA%E7%AE%97%E6%B3%95
// 单源最短路算法，可以处理边是负值的情况，但不能处理含有负权回路的。
// 如果一个点入队的次数超过N次则有负环回路

// poj3259
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1000;
int N;
int matrix[MAXN][MAXN];
int dis[MAXN];
int cnt[MAXN];
bool flag[MAXN];

bool SPFA(int s) {
  memset(flag, 0, sizeof(flag));
  memset(cnt, 0, sizeof(cnt));
  for (int i = 0; i < N; i++) dis[i] = INF;
  std::queue<int> queue;
  queue.push(s);
  flag[s] = true;
  dis[s] = 0;
  cnt[s] += 1;
  while (!queue.empty()) {
    int t = queue.front();
    queue.pop();
    flag[t] = false;
    if (cnt[t] > N) return false;
    for (int i = 0; i < N; i++) {
      if (matrix[t][i] > 0 && dis[t] + matrix[t][i] > dis[i]) {
        dis[i] = dis[t] + matrix[t][i];
        if (flag[i] == false) {
          flag[i] = true;
          queue.push(i);
          cnt[i] += 1;
          if (cnt[i] > N) return false;
        }
      }
    }
  }
  return true;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
