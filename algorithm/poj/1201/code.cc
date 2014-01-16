// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-14 23:17:51
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// poj1201
// 因为边的值都是临界值所以能保证在满足条件下是最小值
// http://wenku.baidu.com/view/f608c6ee5ef7ba0d4a733b99.html

namespace algorithm {

const int MAXN = 50009;
int N;
int min;
int max;
struct Node {
  int u;
  int v;
  int val;
  int next;
} data[MAXN];

int dis[MAXN];
int cnt = 0;

void Add(int u, int v, int val) {
  data[cnt].u = u;
  data[cnt].v = v;
  data[cnt++].val = val;
}

int BellmanFord() {
  memset(dis, 0, sizeof(dis));
  bool flag = true;
  while (flag) {
    flag = false;
    for (int i = 0; i < cnt; i++) {
      if (dis[data[i].v] < dis[data[i].u] + data[i].val) {
        dis[data[i].v] = dis[data[i].u] + data[i].val;
        flag = true;
      }
    }
    for (int i = min; i < max; i++) {
      if (dis[i + 1] < dis[i]) {
        dis[i + 1] = dis[i];
        flag = true;
      }
    }
    for (int i = max; i > min; i--) {
      if (dis[i - 1] < dis[i] - 1) {
        dis[i - 1] = dis[i] - 1;
        flag = true;
      }
    }
  }
  return dis[max];
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    cnt = 0;
    max = INT_MIN;
    min = INT_MAX;
    for (int i = 0; i < N; i++) {
      int b, e, c;
      scanf("%d%d%d", &b, &e, &c);
      getchar();
      Add(b - 1, e, c);
      max = std::max(max, e);
      min = std::min(min, b - 1);
    }
    int t = BellmanFord();
    printf("%d\n", t);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
