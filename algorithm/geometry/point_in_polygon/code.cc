// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-09 14:37:30
// File  : code.cc
// Brief :

/*
http://www.cnblogs.com/hhyypp/archive/2011/12/05/2276984.html
判断点是不是在多边形内
射线法（适用于凸/凹多边形）
注意到如果从P作水平向左的射线的话，如果P在多边形内部，那么这条射线与多边形的交点必为奇数，如果P在多边形外部，则交点个数必为偶数（0也在内）。
所以，我们可以顺序考虑多边形的每条边，求出交点的总个数。
还有一些特殊情况要考虑。假如考虑边(P1,P2)，
1)如果射线正好穿过P1或者P2,那么这个交点会被算作2次，处理的方法端点只属于一条边.
2)如果边水平，则射线要么与其无交点，要么有无数个，这种情况也直接忽略。
4)再判断相交之前，先判断P是否在边(P1,P2)的上面，如果在，则直接得出结论：P再多边形内部。
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
bool InPolygon(std::vector<std::pair<double, double> > & poly, std::pair<double, double> & t) {
  bool inside = false;
  int cnt = 0;
  int n = poly.size();
  for (int i = 0; i < n && !inside; i++) {
    if (t.second < std::min(poly[i].second, poly[(i + 1) % n].second)) continue;
    if (t.second >= std::max(poly[i].second, poly[(i + 1) % n].second)) continue;  // 交点在直线外
    if (poly[i].second == poly[(i + 1) % n].second) continue;  // 边水平
    
    double dx = poly[(i + 1) % n].first - poly[i].first;
    double dy = poly[(i + 1) % n].second - poly[i].second;
    double tx = poly[i].first + dx / dy * (t.second - poly[i].second);
    
    if (fabs(tx - t.first) < 1e-6) inside = true;  // 在直线上
    else if (tx > t.first) cnt++;  // 因为是射线
  }
  if (inside || cnt % 2 == 1) return true;
  else return false;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
