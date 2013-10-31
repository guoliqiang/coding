// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-31 00:52:29
// File  : code.cc
// Brief :

#include "base/public/common_head.h"
#include "../base/base.h"

/*
 * 三角形中心：
 * x = (x1 + x2 + x3) / 3
 * y = (y1 + y2 + y3) / 3
 *
 *
 * http://wenku.baidu.com/link?url=GSaCJanA10MfDNrfTHFhigWAbgfIZ4e_m3gdIAangtTLimerYFav7EEZ5Y0gVXJGbAmF9f9SwFD7N_aNDr3TgNvbYqhhTwiIjp2UfeMAaK_
 * 定理1：由两个图形A，B合并而成的一个图形C，则C的重心必在A的重心与B的重心连接的线段上。(注意，此定理也适用于A B彼此分开，没有公共点的情形)
 * 定理2：由两个A，B合并而成的一个图形C，A的重心为点a, B的重心为点b, C的重心为点c, A的面积为Sa, B的面积为Sb,则下面条件成立：
 * (1)点c 必在线段 ab 上
 * (2) ac * Sa = bc * Sb
 *
 * 尺规做图找重心
 * 1. 三角形重心：三条中位线的交点。
 *
 * 2.四边形的重心作法：连接出四边形的一条对角线，这样四边形就变成两个三角形的组合体，分别作出两个三角形的重心，
 * 并连接两个重心成一条线段AB，同样，连接出四边形的另一条对角线，四边形就变成另外两个三角形的组合体，分别作出
 * 这两个三角形的重心，并连接两个重心成一条线段CD，则线段AB，CD的交点就是四边形的重心。(根据定理1)
 * 
 * 3.五边形的重心作法：连接出五边形的任一条对角线，将五边形分为1个三角形与一个四边形组合体，分别作出三角形的
 * 重心，和四边形的重心，并连成线段AB；连接五边形的另外一条对角形，将五边形分为另1个三角形与四边形的组合体，
 * 分别作出三角形与四边形的重心，并连接成线段CD；则AB、CD的交点就是五边形的重心。
 * 
 * 4、用数学归纳法，对于六边形、七边形，N边形，都可以用上述方法，先连接出一条对角线，将N边形化为一个三角形与(N-1)边形，
 * 或四边形与(N-2)边形，然后分别作出重心，并连接成线段，然后再连接另外一条对象线，分别作出两个组合体的重心并连接成线段，
 * 两条线段的交点就是N边形的重心。
 * 
 * http://ycool.com/post/8cfwv8z
 * 任意多边形匀面重心的计算方法.pdf  左加 《数学通报》
 *
 * 求多边形的重心算法
 * 求多边形重心并不是简单的把求三角形的重心公式推广就行了
 * 算法是在平面上取一点(一般取原点, 这样可以减少很多计算, 而且使思路更清晰^_^)
 * 这样就得到了N个三角形OP[i]P[i+1](其中点的顺序要为逆时针的),
 * 分别求出这N个三角形的重心Ci和面积Ai(注意此处面积是又向面积, 就是用叉乘求面积时保留其正负号)
 * 在求出A = A1+A2+...+AN(同样保留正负号的代数相加)
 * 最终重心C = sigma(Ai+Ci)/A;
 * 
 * 另一种做法：http://www.cnblogs.com/jbelial/archive/2011/08/08/2131165.html (不选原点)
 * */

namespace algorithm {

// 点逆时针存储在v中, 否则计算出的重心符号是反的
// 多边形可以是凸多边形也可以是凹多边形
Point Gravity(std::vector<Point> & v) {
  int n = v.size();
  Point center(0, 0);
  double area = 0;
  // 按 (0,0)-p0 - p1; (0,0) - p1 - p2;  (0,0) - pi - p(i+1) ..
  // 的顺序
  for (int i = 1; i < n; i++) {
    int t = v[i - 1].x * v[i].y - v[i - 1].y * v[i].x;
    area += (double)t * 0.5;
    center.x += (v[i - 1].x + v[i].x) * t;  // 少乘了1/2（三角形的面积中）， 1/3（计算三角形的中心中）
    center.y += (v[i - 1].y + v[i].y) * t;
  }
  int t = v[n - 1].x * v[0].y - v[n - 1].y * v[0].x;
  center.x += (v[n - 1].x + v[0].x) * t;
  center.y += (v[n - 1].y + v[0].y) * t;
  area += t * 0.5;
  
  center.x /= 6 * area;  // 除以少乘的6
  center.y /= 6 * area;
  return center;
}

void Read(std::vector<Point> & v) {
  v.push_back(Point(0, -4));
  v.push_back(Point(2, -2));
  v.push_back(Point(-2, 0));
  v.push_back(Point(3, 2));
  v.push_back(Point(0, 2));
  v.push_back(Point(-1, 4));
  v.push_back(Point(-4, 0));
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<Point> v;
  Read(v);
  Point c = Gravity(v);
  LOG(INFO) << c.x << " " << c.y;
  return 0;
}
