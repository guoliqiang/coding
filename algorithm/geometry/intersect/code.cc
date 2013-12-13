// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-30 23:20:14
// File  : code.cc
// Brief :

// http://wenku.baidu.com/view/296efee20975f46526d3e10f.html
#include "base/public/common_head.h"
#include "../base/base.h"

namespace algorithm {
bool SegmentInter(const Point & a, const Point & b,
                  const Point & c, const Point & d) {
  // 快速排斥实验
  if (std::min(a.x, b.x) > std::max(c.x, d.x) ||
      std::min(a.y, b.y) > std::max(c.y, d.y) ||
      std::min(c.x, d.x) > std::max(a.x, b.x) ||
      std::min(c.y, d.y) > std::max(a.y, b.y)) return false;
  // 跨立实验，也就是分别验证一个线段的两个点是不是分别处于
  // 另一条线段所在直线的两侧，基于叉积实现
  // 如果处于两侧的话 a -> c -> b   a - >  d -> b 一个是左旋转，
  // 另一个是右旋转
  int h = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
  int i = (b.x - a.x) * (d.y - a.y) - (b.y - a.y) * (d.x - a.x);
  int j = (d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x);
  int k = (d.x - c.x) * (b.y - c.y) - (d.y - c.y) * (b.x - c.x);
  return h * i <= 0 && j * k <= 0;
}

bool LineInter(const Point & a, const Point & b,
               const Point & c, const Point & d) {
  double temp = Cross(Sub(a, b), Sub(c, d));
  if (temp >= ESP) return true;
  else {
    double temp2 = Cross(Sub(b, a), Sub(c, b));  // 判断a b c 是不是在一条直线上
    if (temp2 < ESP) return true;  // 重合
    else return false;
  }
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  return 0;
}
