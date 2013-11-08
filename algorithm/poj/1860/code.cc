// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-07 22:27:37
// File  : code.cc
// Brief :

/*
 * 在图上做dp的方法(最小费用流也是这样的),如果有优化方案的话应该是在queue选择front元素的时候
 * 可以使用proority queue.
 *
 * Accepted 288K  32MS
 *
 * use double, never use float, it will leading to WA
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {

const int INF = 0x3f3f3f3f;
const int MAX = 110;
int N = 0;
int E = 0;
double r[MAX][MAX];
double c[MAX][MAX];
double dis[MAX];

int begin = 0;
double val = 0;

// 基于在存在有负权边计算单元最短路的bellman ford算法基础上
// 判断是不是含有正权回路
bool BellmanFord() {
  for (int i = 0; i < N; i++) dis[i] = 0;
  dis[begin] = val;
  
  for (int k = 1; k < N; k++) {
    bool flag = false;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (r[i][j] > 0) {
          double t = (dis[i] - c[i][j]) * r[i][j];
          if (t > dis[j]) {
            dis[j] = t;
            flag = true;
          }
        }
      }
    }
    if (!flag) break;
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (r[i][j] > 0 && (dis[i] - c[i][j]) * r[i][j] > dis[j]) return true;
    }
  }
  return false;
}

bool Increase() {
  for (int i = 0; i < N; i++) dis[i] = 0;
  dis[begin] = val;

  std::queue<int> queue;
  for (int i = 0; i < N; i++) {
    if (r[begin][i] > 0) {
      double t = (dis[begin] - c[begin][i]) * r[begin][i];
      if (t > dis[i]) {
        dis[i] = t;
        queue.push(i);
      }
    }
  }
  while (!queue.empty()) {
    int t = queue.front();
    queue.pop();
    for (int i = 0; i < N; i++) {
      if (r[t][i] > 0) {
        double cur = (dis[t] - c[t][i]) * r[t][i];
        if (cur > dis[i]) {
          dis[i] = cur;
          queue.push(i);
          if (i == begin) return true;
        }
      }
    }
  }
  return false;
}

void Read() {
  int n, m, s;
  double v;
  scanf("%d%d%d%lf", &n, &m, &s, &v);
  getchar();
  N = n;
  begin = s - 1;
  val = v;
  E = 0;
  memset(r, 0, sizeof(r));
  memset(c, 0, sizeof(c));
  for (int i = 0; i < m; i++) {
    int x, y;
    double r_xy, r_yx, c_xy, c_yx;
    scanf("%d%d%lf%lf%lf%lf", &x, &y, &r_xy, &c_xy, &r_yx, &c_yx);
    getchar();
    r[x - 1][y - 1] = r_xy;
    r[y - 1][x - 1] = r_yx;
    c[x - 1][y - 1] = c_xy;
    c[y - 1][x - 1] = c_yx;
    E += 2;
    // LOG(INFO) << x  << " " << y << " " << r_xy << " " << r_yx << " " << c_xy << " " << c_yx;
  }
  // if (Increase()) printf("YES\n");
  if (BellmanFord()) printf("YES\n");
  else printf("NO\n");
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
