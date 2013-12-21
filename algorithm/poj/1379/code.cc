// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-20 01:16:06
// File  : code.cc
// Brief :

// Accepted 220K  547MS
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1004;
const double pi = 3.1415926;
int N;
const int NUM = 15;
struct Node {
  double x;
  double y;
  double v;
} hole[MAXN], data[NUM];

int X;
int Y;

double Dis(double x, double y) {
  double rs = INF;
  for (int i = 0; i < N; i++) {
    double t = pow(x - hole[i].x, 2) + pow(y - hole[i].y, 2);
    rs = std::min(rs, sqrt(t));
  }
  return rs;
}

void Solve() {
  for (int i = 0; i < NUM; i++) {
    data[i].x = double(rand() % 1000 + 1) / 1000.00 * X;
    data[i].y = double(rand() % 1000 + 1) / 1000.00 * Y;
    data[i].v = Dis(data[i].x, data[i].y);
  }
  double t = std::max(X, Y);
  while (t > 0.0001) {
    for (int i = 0; i < NUM; i++) {  // 这两个循环的次数可以控制最优值的精度，循环次数约多，得到最优值的概率越大
      for (int j = 0; j < NUM + 15; j++) {  // 循环15次 case 1不能得到最优解
        double theta = double(rand() % 1000 + 1) / 1000.00 * 10 * pi;
        double dx = cos(theta) * t;
        double dy = sin(theta) * t;
        double tx = data[i].x + dx;
        double ty = data[i].y + dy;
        if (tx < 0 || tx > X || ty < 0 || ty > Y) continue;
        double dis = Dis(tx, ty);
        if (dis > data[i].v) {
          data[i].x += dx;
          data[i].y += dy;
          data[i].v = dis;
        }
      }
    }
    t *= 0.8;
  }
  int idx = 0;
  for (int i = 1; i < NUM; i++) {
    if (data[i].v > data[idx].v) idx = i;
  }
  printf("The safest point is (%.1lf, %.1lf).\n", data[idx].x, data[idx].y);
}

void Read() {
  int T = 0;
  scanf("%d", &T);
  getchar();
  for (int k = 0; k < T; k++) {
    scanf("%d%d%d", &X, &Y, &N);
    getchar();
    for (int i = 0; i < N; i++) {
      int x, y;
      scanf("%d%d", &x, &y);
      getchar();
      hole[i].x = x;
      hole[i].y = y;
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
