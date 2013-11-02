// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-30 23:21:22
// File  : base.h
// Brief :

#ifndef  __BASE_H_
#define  __BASE_H_

namespace algorithm {
struct Point {
  double x;
  double y;
  explicit Point (double xi = 0, double yi = 0) : x(xi), y(yi){}
};

double Distance(const Point & a, const Point & b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Point Sub(const Point & a, const Point & b) {
  return Point(a.x - b.x, a.y - b.y);
}

Point Add(const Point & a, const Point & b) {
  return Point(a.x + b.x, a.y + b.y);
}

double Cross(const Point & a, const Point & b) {
  return a.x * b.y - a.y * b.x;
}
}  // namespace algorithm

#endif  // __BASE_H_
