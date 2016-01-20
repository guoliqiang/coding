// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 12:50:58
// File  : code.cc
// Brief :
// 线段树求矩阵重叠
// http://www.aerchi.com/csdn-code-mishifangxiangdefeng-7909307.html
// 思想：根据扫描线更新线段树
// facebook:求一个矩阵重叠最多的位置
#include "base/public/common_ojhead.h"

namespace algorithm {
struct Interval {
 int start;
 int end;
 Interval(int s, int e) : start(s), end(e) {}
};

int MinMeetingRoom(std::vector<Interval> & vec) {
  std::vector<std::pair<int, int> > t;
  for (int i = 0; i < vec.size(); i++) {
    t.push_back(std::make_pair(vec[i].start, 1));
    t.push_back(std::make_pair(vec[i].end, -1));
  }
  std::sort(t.begin(), t.end());
  int ans = 0;
  int cur = 0;
  for (int i = 0; i < t.size(); i++) {
    if (t[i].second == 1) cur++;
    else cur--;
    ans = std::max(cur, ans);
  }
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<Interval> vec;
  vec.push_back(Interval(0, 8));
  vec.push_back(Interval(5, 10));
  vec.push_back(Interval(15, 20));
  LOG(INFO) << MinMeetingRoom(vec);
  return 0;
}
