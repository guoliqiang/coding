// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-30 20:22:36
// File  : code.cc
// Brief :

/*
 * 给定平面上的一个点集，找出一个最小点集顺次连结形成一个凸多边形，使得点集中的点皆在此多边形内或
 * 此多边形上，这个凸多边形就是给定点集的二维凸包.
 * http://wenku.baidu.com/view/e0db03edf8c75fbfc77db2ce.html?from=related
 *
 * http://wenku.baidu.com/view/a5bb0072a417866fb84a8e98.html
 *
 * (ax.i, ay.j, az.k) * (bx.i, by.j, bz.k)
 * = ax bx.i * i + ax by.i * j + ax bz.i * k
 * + ay bx.j * i + ay by.j * j + ay bz.j * k
 * + az bx.k * i + az by.k * j + az bz.k * k
 *
 * 
 * 由于
 * i * i = 0 j * j = 0 z * z = 0
 *
 * k * i = j  j * k = i  i * j = k
 *
 * =>
 *   ax by.k  + ax bz.-j
 * + ay bx.-k   + ay bz.i
 * + az bx.j  + az by.-i
 *
 * ==>
 * (ay bz - az by) i +
 * (az bx - ax bz) j +
 * (ax by - ay bx) k
 *
 *
 *     |  k
 *     |  
 *     ----  j
 *    /   
 *   / i
 *
 *
 * */
#include "base/public/common_head.h"
#include "../base/base.h"

namespace algorithm {

std::vector<Point> stack;


/*  判断 base->a->b 是不是左旋转

  左旋转
    b
       \
        a
       /
  base

  右旋转
        b
      /
    a
      \
       base
*/

// 通过向量a - base； b - base的叉积的方向判断
// 注意点积和叉积的区别
// http://wenku.baidu.com/link?url=kmnAck6eUMcnQXmbMYa6pLYajD-O1pOQ0D5d5nadJRsyPPVXgzU1D_EJPveZPgOIjf5YI08p-USxvo2QO48M3g_Sv2kSLNJPrt0GmkJFK6C
// a . b = |a|*|b|* cos(a,b)
// a 叉 b = c (叉是乘法符号；c是一个向量。可以看到叉积的结果是一个向量)
// 向量c的大小为|a|*|b| * sin(a, b),方向符合右手法则
// 
// 可以更具c向量在z轴上的方向确定是不是左旋转
bool LeftRotate(const Point & a, const Point & b, const Point & base, int * rs = NULL) {
  int t = (a.x - base.x) * (b.y - base.y)  - (a.y - base.y) * (b.x - base.x);
  if (rs != NULL) *rs = t;
  return t < 0 ? true : false;
}

// 寻找左下点p0
bool Cmp1(const Point & a, const Point & b) {
  return a.y == b.y ? a.x < b.x : a.y < b.y;
}

// 按p0px的极角的大小排序，p0px的极角为p0px与x轴的夹角
// 极角相同时按p0px的长度排序
bool Cmp2(const Point & a, const Point & b) {
  int cross = -1;
  if (LeftRotate(a, b, stack[0], &cross)) return true;
  if (cross == 0) return Distance(a, stack[0]) < Distance(b, stack[0]);
  return false;
}

int Graham() {
  std::sort(stack.begin(), stack.end(), Cmp1);
  std::sort(stack.begin() + 1, stack.end(), Cmp2);
  if (stack.size() <= 3) return stack.size();
  // 至少3个点才能组成一个多边形
  int top = 2;
  for (int i = 3; i < stack.size(); i++) {
    while (top > 1 && !LeftRotate(stack[top], stack[i], stack[top - 1])) top--;
    stack[++top] = stack[i];
  }
  return top;
}
void Read() {
  stack.push_back(Point(1, 1));
  stack.push_back(Point(2, 2));
  stack.push_back(Point(1, 2));
  stack.push_back(Point(0, 3));
}
}  // namespace algorithm


// 凸包相关算法，如凸包间的最小（最大）距离
// http://cgm.cs.mcgill.ca/~orm/rotcal.frame.html
namespace algorithm {
// 凸包直径
// 旋转卡壳算法
// http://www.cppblog.com/staryjy/archive/2009/11/19/101412.html

// 0 ~n -1 按逆时针的顺序存储在v中
double RotatingCalipers(std::vector<Point> v) {
  int n = v.size();
  v.push_back(v[0]);
  double rs = 0;
  int last = 1;
  for (int i = 0; i < n; i++) {
    Point t = Sub(v[i + 1], v[i]);
    while (Cross(t, Sub(v[last + 1], v[last])) > 0) {
      last = (last + 1) % n;
    }
    double foo = std::max(Distance(v[i], v[last]), Distance(v[i + 1], v[last + 1]));
    rs = std::max(rs, foo);
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<Point> v;
  v.push_back(Point(2, 0));
  v.push_back(Point(-2, 0));
  v.push_back(Point(0, 2));
  v.push_back(Point(-1, -2));
  v.push_back(Point(1, -2));
  LOG(INFO) << RotatingCalipers(v);
  return 0;
  Read();
  int n = Graham();
  for (int i = 0; i <= n; i++) {
    LOG(INFO) << "(" << stack[i].x << "," << stack[i].y << ")";
  }
  return 0;
}
