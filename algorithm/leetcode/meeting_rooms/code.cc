// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 12:50:58
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
struct Interval {
 int start;
 int end;
 Interval(int s, int e) : start(s), end(e) {}
};

bool Compare(const Interval & a, const Interval & b) {
  return a.start < b.start;
}

bool CanAttendAll(std::vector<Interval> & vec) {
  int n = vec.size();
  std::sort(vec.begin(), vec.end(), Compare);
  for (int i = 1; i < n; i++) {
    if (vec[i].start < vec[i - 1].end) return false;
  }
  return true;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<Interval> vec;
  vec.push_back(Interval(0, 30));
  vec.push_back(Interval(5, 10));
  vec.push_back(Interval(15, 20));
  LOG(INFO) << CanAttendAll(vec);
  return 0;
}
