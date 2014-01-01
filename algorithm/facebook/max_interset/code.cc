// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-01 15:28:07
// File  : code.cc
// Brief :

/*
giving lots of intervals [ai, bi], find a point intersect with the most number of intervals
http://www.careercup.com/question?id=14851686

store each boundary value as a separate element in an array with the information whether it's the start boundary or end boundary. 
Sort this array by boundary value. 
initialize an integer overLapCount = 0; 
Traverse this array from start to finish 
whenever you cross a "Start" boundary, increment overlapCount 
whenever you cross an "End" boundary, decrement overlapCount 
store the maxOverLapCountSoFar 
at the end of the traversal you 

*/
#include "base/public/common_ojhead.h"

namespace algorithm {
struct Interval {
  int b;
  int e;
};

struct Node {
  int val;
  bool flag;
  Node(int v, bool f) : val(v), flag(f){}
  bool operator < (const Node & b) const {
    return val < b.val;
  }
};

int MaxInterset(std::vector<Interval> & v) {
  int rs = 0;
  std::vector<Node> vec;
  for (int i = 0; i < v.size(); i++) {
    vec.push_back(Node(v[i].b, true));
    vec.push_back(Node(v[i].e, false));
  }
  std::sort(vec.begin(), vec.end());
  int t = 0;
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i].flag) t++;
    else t--;
    rs = std::max(rs, t);
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
