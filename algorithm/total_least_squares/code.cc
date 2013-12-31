// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-29 21:35:07
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

/*
最小二成法求距离各个点距离之和最近的一条直线
y =  a0 * x + a1
目标函数
S = Sum[(a0 * xi + a1) - yi]^2
对a0求偏导，是其等于0  ==》
Sum[(a0 * xi + a1) - yi] * xi  = 0
==>
a1 = [Sum(xi * yi) - a0 * Sum(xi^2)] / Sum(xi)

对a1求偏导，是其等于0  ==》
a0 = (Sum(yi) - n * a1) / Sum(xi)
==>
a0 = (Sum(yi) * Sum(xi) - n * Sum(xi * yi)) / ((Sum(xi))^2 - n * Sum(xi ^ 2))

*/

namespace algorithm {
// y = a0x + a1
std::pair<double, double> BestLine(std::vector<std::pair<double, double> > & v) {
  double s_x_y = 0;
  double s_x = 0;
  double s_y = 0;
  double s_x2 = 0;
  for (int i = 0; i < v.size(); i++) {
    s_x_y += v[i].first * v[i].second;
    s_x += v[i].first;
    s_y += v[i].second;
    s_x2 += v[i].first * v[i].first;
  }
  double t1 = s_y * s_x - v.size() * s_x_y;
  double t2 = s_x * s_x - v.size() * s_x2;
  double a0 = t2 == 0 ? 0 : t1 / t2;

  t1 = s_x_y - a0 * s_x2;
  t2 = s_x;
  double a1 = t2 == 0 ? 0 : t1 / t2;
  return std::make_pair(a0, a1);
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  std::vector<std::pair<double, double> > v;
  v.push_back(std::make_pair(2, 2));
  v.push_back(std::make_pair(4, 11));
  v.push_back(std::make_pair(6, 28));
  v.push_back(std::make_pair(8, 40));
  std::pair<double, double> rs = BestLine(v);
  LOG(INFO) << rs.first << " " << rs.second;
  return 0;
}
