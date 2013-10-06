// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-06 18:30:40
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

struct Cmp {
 public:
  bool operator() (const std::pair<double, double> & a,
                   const std::pair<double, double> & b) const {
    std::pair<double, double> t = b;
    if (fabs(a.first - b.first) < 0.001) {
      t.first = a.first;
    }
    if (fabs(a.second - b.second) < 0.001) {
      t.second = a.second;
    }
    if (a.first != t.first) return a.first < t.first;
    return a.second < t.second;
  }
};
int MaxPointsInLine(std::vector<std::pair<double, double> > & v) {
  std::map<std::pair<double, double>, int, Cmp > tmap;
  int max = 0;
  for (int i = 0; i < v.size(); i++) {
    for (int j = i + 1; j < v.size(); j++) {
      double k = (v[i].second - v[j].second) / (v[i].first - v[j].first);
      double b = v[i].second - k * v[i].first;
      std::pair<double, double> t = std::make_pair(k, b);
      if (tmap.count(t)) tmap[t]++;
      else tmap[t] = 1;
      max = std::max(max, tmap[t]);
    } 
  }
  return max;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::pair<double, double> > v;
  v.push_back(std::make_pair(10.0001, 20.0001));
  v.push_back(std::make_pair(10.0002, 20.0004));
  v.push_back(std::make_pair(10.0003, 20.0003));
  v.push_back(std::make_pair(1.0004, 20.0002));
  v.push_back(std::make_pair(5.0004, 7.0002));
  LOG(INFO) << MaxPointsInLine(v);
  return 0;
}
