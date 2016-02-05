// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 13:48:44
// File  : code.cc
// Brief :

/*
Given a collection of intervals, merge all overlapping intervals.
For example,
Given [1,3],[2,6],[8,10],[15,18],
return [1,6],[8,10],[15,18].
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 16/16 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 64 milli secs
 * Progress: 168/168 test cases passed.
 *
 * */

#include <vector>
#include <algorithm>
#include "base/public/logging.h"

namespace algorithm {

struct Interval {
  int start;
  int end;
  Interval() : start(0), end(0) {}
  Interval(int s, int e) : start(s), end(e) {}
};

bool Compare(const Interval & a, const Interval & b) {
  return a.start < b.start;
}

std::vector<Interval> Merge(std::vector<Interval> & v) {
  std::vector<Interval> rs;
  if (v.size() == 0) return rs;
  std::sort(v.begin(), v.end(), Compare);
  Interval pre = v[0];
  for (int i = 1; i < v.size(); i++) {
    if (pre.end < v[i].start) {
      rs.push_back(pre);
      pre = v[i];
    } else {
      if (pre.end >= v[i].start && pre.end <= v[i].end) {
        pre.end = v[i].end;
      }
    }
  }
  rs.push_back(pre);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

namespace clear {
bool Cmp(const Interval & x, const Interval & y) {
    return x.start < y.start;
}

std::vector<Interval> merge(std::vector<Interval>& intervals) {
    std::sort(intervals.begin(), intervals.end(), Cmp);
    std::vector<Interval> ans;
    for (int i = 0; i < intervals.size(); i++) {
        if (ans.size() == 0) ans.push_back(intervals[i]);
        else {
            if (ans.back().end < intervals[i].start) ans.push_back(intervals[i]);
            else ans.back().end = std::max(ans.back().end, intervals[i].end);  // bug fixed, add max
        }
    }
    return ans;
}
}  // namespace clear

int main(int argc, char** argv) {
  std::vector<Interval> foo;
  foo.push_back(Interval(1, 3));
  foo.push_back(Interval(2, 6));
  foo.push_back(Interval(8, 10));
  foo.push_back(Interval(15, 18));

  std::vector<Interval> rs = Merge(foo);

  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i].start << " " << rs[i].end;
  }
  return 0;
}
