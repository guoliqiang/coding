// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-05 13:49:31
// File  : code.cc
// Brief :

// http://www.careercup.com/question?id=7894677

#include "base/public/common_ojhead.h"

namespace algorithm {
bool Cmp(const std::pair<int, int> & x, const std::pair<int, int> & y) {
  return x.second < y.second;
}

int Overlap(const std::pair<int, int> & x, const std::pair<int, int> & y) {
  int t = std::min(x.second, y.second) - std::max(x.first, y.first) + 1;
  return t < 0 ? 0 : t;
}

// 不能有完全覆盖的interal，否则剔除大的interval，只保留小的，因为满足小的一定能满足大的interval
std::vector<std::pair<int, int> > EventSchedule(std::vector<std::pair<int, int> > & vec, int t) {
  std::vector<std::pair<int, int> > rs;
  std::sort(vec.begin(), vec.end(), Cmp);
  std::pair<int, int> tmp = std::make_pair(0, 0);
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i].second - vec[i].first + 1< t) {
      return std::vector<std::pair<int, int> >();
    }
    if (i == 0) {
      tmp.second = vec[i].second;
      tmp.first = vec[i].second - t + 1;
    } else {
      int foo = Overlap(tmp, vec[i]);
      if (foo != t) {
        rs.push_back(tmp);
        tmp.second = vec[i].second;
        tmp.first = vec[i].second - (t - foo) + 1;
      }
    }
  }
  if (vec.size() != 0) rs.push_back(tmp);
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  std::vector<std::pair<int, int> > vec;
  vec.push_back(std::make_pair(1, 4));
  vec.push_back(std::make_pair(2, 9));
  vec.push_back(std::make_pair(5, 12));
  vec.push_back(std::make_pair(7, 8));
  std::vector<std::pair<int, int> > rs = EventSchedule(vec, 2);
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i].first << " " << rs[i].second;
  }
  return 0;
}
