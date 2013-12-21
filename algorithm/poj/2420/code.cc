// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-20 00:37:47
// File  : code.cc
// Brief :

// Accepted 200K  141MS
// 模拟退火算法

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 105;
const double pi = 3.1415926;
int N;
const int NUM = 30;
struct Node {
  double x;
  double y;
  double v;
} computer[MAXN], data[NUM];  // 避免陷入局部最优解中，随机选择NUM个点

int max_x;
int min_x;
int max_y;
int min_y;

double Dis(const Node & x) {
  double rs = 0;
  for (int i = 0; i < N; i++) {
    double t = pow(x.x - computer[i].x, 2) + pow(x.y - computer[i].y, 2);
    rs += sqrt(t);
  }
  return rs;
}

void Solve() {
  for (int i = 0; i < NUM; i++) {
    data[i].x = min_x + double(rand() % 1000 + 1) / 1000.00 * (max_x - min_x);
    data[i].y = min_y + double(rand() % 1000 + 1)/ 1000.00 * (max_y - min_y);
    data[i].v = Dis(data[i]);
  }
  double d = std::max(max_x, max_y);
  while (d > 0.001) {
    for (int i = 0; i < NUM; i++) {
      for (int j = 0; j < NUM; j ++) {
        double theta = double(rand() % 1000 + 1) / 1000.00 * 10 * pi;
        double dx = cos(theta) * d;
        double dy = sin(theta) * d;
        // 费马点一定在这个矩形内，这个判断条件也可以省略
        if (data[i].x + dx < 0 || data[i].x + dx > max_x || data[i].y + dy < 0 || data[i].y + dy > max_y) continue;
        Node foo;
        foo.x = data[i].x + dx;
        foo.y = data[i].y + dy;
        double t = Dis(foo);
        if (t < data[i].v) {
          data[i].x += dx;
          data[i].y += dy;
          data[i].v = t;
        }
      }
    }
    d *= 0.8;
  }
  double rs = INF;
  for (int i = 0; i < NUM; i++) {
    rs = std::min(rs, data[i].v);
  }
  printf("%.0lf\n", rs);
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    max_x = -INF;
    min_x = INF;
    max_y = -INF;
    min_y = INF;
    for (int i = 0; i < N; i++) {
      int x, y;
      scanf("%d%d", &x, &y);
      getchar();
      computer[i].x = x;
      computer[i].y = y;
      max_x = std::max(max_x, x);
      min_x = std::min(min_x, x);
      max_y = std::max(max_y, y);
      min_y = std::min(min_y, y);
    }
    Solve();
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
