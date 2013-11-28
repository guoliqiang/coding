// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 13:48:44
// File  : code.cc
// Brief :

/*
Given a set of non-overlapping intervals, insert a new interval into the intervals (merge if necessary).
You may assume that the intervals were initially sorted according to their start times.

Example 1:
Given intervals [1,3],[6,9], insert and merge [2,5] in as [1,5],[6,9].

Example 2:
Given [1,2],[3,5],[6,7],[8,10],[12,16], insert and merge [4,9] in as [1,2],[3,10],[12,16].

This is because the new interval [4,9] overlaps with [3,5],[6,7],[8,10].
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 20/20 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 68 milli secs
 * Progress: 151/151 test cases passed.
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

// find the first bigger
void Find(std::vector<Interval> & v, int f, int * rs) {
  if (v[0].start >= f) {
    *rs = -1;
    return;
  }
  if (v[v.size() - 1].start <= f) {
    *rs = v.size();
    return;
  }
  int b = 0;
  int e = v.size();
  while (b < e) {
    int mid = b + (e - b) / 2;
    if (v[mid].start == f ||
        (mid != 0 && v[mid].start > f && v[mid - 1].start < f)) {
      *rs = mid;
      return;
    }
    if (v[mid].start > f) e = mid;
    else b = mid + 1;
  }
}

std::vector<Interval> Merge(std::vector<Interval> & v) {
  std::vector<Interval> rs;
  if (v.size() == 0) return rs;
  // not need to sort
  // std::sort(v.begin(), v.end(), Compare);
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

std::vector<Interval> Insert(std::vector<Interval> & v, Interval & k) {
  std::vector<Interval> foo;
  if (v.size() == 0) {
    foo.push_back(k);
    return foo;
  }

  int idx = -1;
  Find(v, k.start, &idx);
  if (idx == -1) {
    foo.push_back(k);
    foo.insert(foo.end(), v.begin(), v.end());
  } else {
    if (idx == v.size()) {
      foo.insert(foo.end(), v.begin(), v.end());
      foo.push_back(k);
    } else {
      for (int i = 0; i < v.size(); i++) {
        if (i == idx) foo.push_back(k);
        foo.push_back(v[i]);
      }
    }
  }

  return Merge(foo);
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
using namespace std;
bool Cmp(const Interval & a, const Interval & b) {
  return a.start < b.start;
}
//O(n*log(n))
vector<Interval> insert(vector<Interval> &intervals, Interval newInterval) {
        // Note: The Solution object is instantiated only once and is reused by each test case.
        intervals.push_back(newInterval);
        std::sort(intervals.begin(), intervals.end(), Cmp);
        std::vector<Interval> rs;
        Interval pre = intervals[0];
        for (int i = 1; i < intervals.size(); i++) {
            if (intervals[i].start <= pre.end) {
              if (intervals[i].end > pre.end) pre.end = intervals[i].end;
            } else {
                rs.push_back(pre);
                pre = intervals[i];
            }
        }
        rs.push_back(pre);
        return rs;
    }
}  // namespace twice

namespace third {
// O(n)
std::vector<Interval> Insert(std::vector<Interval> &intervals, Interval newInterval) {
  intervals.push_back(newInterval);
  int n = intervals.size();
  for (int i = n - 1; i > 0; i--) {
    if (intervals[i - 1].start > intervals[i].start)
      std::swap(intervals[i - 1], intervals[i]);
    else break;
  }
  std::vector<Interval> rs;
  Interval pre = intervals[0];
  for (int i = 1; i < n; i++) {
    if (intervals[i].start <= pre.end) pre.end = std::max(pre.end, intervals[i].end);
    else {
      rs.push_back(pre);
      pre = intervals[i];
    }
  }
  rs.push_back(pre);
  return rs;
}

}  // namespace third

int main(int argc, char** argv) {
  std::vector<Interval> foo;
  foo.push_back(Interval(1, 2));
  foo.push_back(Interval(3, 5));
  foo.push_back(Interval(6, 7));
  foo.push_back(Interval(8, 10));
  foo.push_back(Interval(12, 16));
  Interval k(4, 9);
  std::vector<Interval> rs = Insert(foo, k);

  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i].start << " " << rs[i].end;
  }
  return 0;
}
