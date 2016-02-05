// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 00:05:42
// File  : code.cc
// Brief : https://segmentfault.com/a/1190000003894693

/*
A group of two or more people wants to meet and minimize the total travel distance. You are given a 2D grid of values 0 or 1, where each 1 marks the home of someone in the group. The distance is calculated using Manhattan Distance, where distance(p1, p2) = |p2.x - p1.x| + |p2.y - p1.y|.

For example, given three people living at (0,0), (0,4), and (2,2):

1 - 0 - 0 - 0 - 1
|   |   |   |   |
0 - 0 - 0 - 0 - 0
|   |   |   |   |
0 - 0 - 1 - 0 - 0
The point (0,2) is an ideal meeting point, as the total travel
distance of 2+2+2=6 is minimal. So return 6.
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
int MinTotalDistance(std::vector<std::vector<int> > & grid) {
  int m = grid.size();
  int n = grid[0].size();

  std::vector<int> x;
  std::vector<int> y;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (grid[i][j] == 1) {
        x.push_back(i);
        y.push_back(j);
      }
    }
  }
  int ans = 0;
  std::sort(y.begin(), y.end());
  for (int i = 0; i < x.size(); i++) {
    ans += fabs(x[i] - x[x.size() / 2]);
  }
  for (int i = 0; i < y.size(); i++) {
    ans += fabs(y[i] - y[y.size() / 2]);
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
