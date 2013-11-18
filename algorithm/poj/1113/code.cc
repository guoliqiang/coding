// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-17 21:06:17
// File  : code.cc
// Brief :

/*
 * Accepted 164K  32MS
 *
 * */


#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 1010;
int N = 0;
int top = 0;
int stack[MAXN];
struct Point {
  int x;
  int y;
  Point(int x_ = 0, int y_ = 0): x(x_), y(y_) {}
} point[MAXN];

bool Cmp1(const int & x, const int & y) {
  Point & p1 = point[x];
  Point & p2 = point[y];
  return (p1.y < p2.y) || ((p1.y == p2.y) && (p1.x < p2.x));
}

// p1 - base 叉 p2 - base
double Cross(const Point & p1, const Point & p2, const Point & base) {
  int t = (p1.x - base.x) * (p2.y - base.y) - (p1.y - base.y) * (p2.x - base.x);
  return t;
}

double Distance(const Point & p1, const Point & p2) {
  double t = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
  return sqrt(t);
}

bool Cmp2(const int & x, const int & y) {
  int t = Cross(point[x], point[y], point[stack[0]]);
  if (t > 0) {
    return true;
  } else if (t == 0) {
    return Distance(point[x], point[stack[0]]) < Distance(point[y], point[stack[0]]);
  } else {
    return false;
  }
}

void Converx() {
  std::sort(stack, stack + N, Cmp1);
  std::sort(stack + 1, stack + N, Cmp2);
  top = 1;
  for (int i = 2; i < N; i++) {
    while (top && Cross(point[stack[i]], point[stack[top]], point[stack[top - 1]]) > 0) {
      top--;
    }
    stack[++top] = stack[i];
  }
  stack[++top] = stack[0];
}

int Len(int r) {
  double rs = 2 * 3.1415926 * r;
  for (int i = 0; i < top; i++) {
    rs += Distance(point[stack[i]], point[stack[i + 1]]);
  }
  return rs + 0.5;
}

void Read() {
  int n, r;
  while (scanf("%d%d", &n, &r) != EOF) {
    getchar();
    N = n;
    memset(stack, 0, sizeof(stack));
    memset(point, 0, sizeof(point));
    for (int i = 0; i < n; i++) {
      int x, y;
      scanf("%d%d", &x, &y);
      getchar();
      point[i].x = x;
      point[i].y = y;
      stack[i] = i;
    }
    Converx();
    int t = Len(r);
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
