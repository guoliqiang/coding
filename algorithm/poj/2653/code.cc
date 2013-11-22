// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-11-22 02:56:34
// File  : code.cc
// Brief :

/*
 * Accepted  3304K  547MS
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 100009;
int N = 0;

struct Point {
  double x;
  double y;
};
struct Line {
  Point left;
  Point right;
};

Line data[MAXN];

double Cross(const Point & a, const Point & b, const Point & c) {
  return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

bool Inter(const Line & a, const Line & b) {
  if ((std::min(a.left.x, a.right.x) > std::max(b.left.x, b.right.x)) ||
      (std::min(a.left.y, a.right.y) > std::max(b.left.y, b.right.y)) ||
      (std::min(b.left.x, b.right.x) > std::max(a.left.x, a.right.x)) ||
      (std::min(b.left.y, b.right.y) > std::max(a.left.y, a.right.y))) return false;
  double h = Cross(a.left, b.left, a.right);
  double j = Cross(a.left, b.right, a.right);
  double k = Cross(b.left, a.left, b.right);
  double l = Cross(b.left, a.right, b.right);
  return h * j <= EPS && k * l <= EPS;
}

void Compute() {
  printf("Top sticks: ");
  for (int i = 0; i < N - 1; i++) {
    for (int j = i + 1; j < N; j++) {
      if (Inter(data[i], data[j])) break;
      if (j == N - 1) printf("%d, ", i + 1); 
    }
  }
  printf("%d.\n", N);  // 最后一个肯定是top
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF && n != 0) {
    getchar();
    N = n;
    double x1, y1, x2, y2;
    for (int i = 0; i < n; i++) {
      scanf("%lf%lf%lf%lf", &x1, &y1, &x2, &y2);
      getchar();
      data[i].left.x = x1;
      data[i].left.y = y1;
      data[i].right.x = x2;
      data[i].right.y = y2;
    }
    Compute();
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
