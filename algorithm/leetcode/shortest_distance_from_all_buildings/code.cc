// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-08 15:31:38
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void BFS(std::vector<std::vector<int> > & grid,
         std::vector<std::vector<int> > & dist,
         std::vector<std::vector<int> > & num,
         int bi, int bj, int m, int n) {
  std::queue<std::pair<int, int> > queue;
  std::set<std::pair<int, int> > vis;
  queue.push(std::make_pair(bi, bj));
  int level = 0;
  vis.insert(std::make_pair(bi, bj));

  while (queue.size()) {
    int cnt = queue.size();
    for (int i = 0; i < cnt; i++) {
      std::pair<int, int> cur = queue.front();
      queue.pop();
      if (grid[cur.first][cur.second] == 0) {
        dist[cur.first][cur.second] += level;
        num[cur.first][cur.second]++;
      }
      if (cur.first + 1 < m && grid[cur.first + 1][cur.second] == 0 &&
          !vis.count(std::make_pair(cur.first + 1, cur.second))) {
        queue.push(std::make_pair(cur.first + 1, cur.second));
        vis.insert(std::make_pair(cur.first + 1, cur.second));
      }
      if (cur.first - 1 >= 0 && grid[cur.first - 1][cur.second] == 0 &&
          !vis.count(std::make_pair(cur.first - 1, cur.second))) {
        queue.push(std::make_pair(cur.first - 1, cur.second));
        vis.insert(std::make_pair(cur.first - 1, cur.second));
      }
      if (cur.second + 1 < n && grid[cur.first][cur.second + 1] == 0 &&
          !vis.count(std::make_pair(cur.first, cur.second + 1))) {
        queue.push(std::make_pair(cur.first, cur.second + 1));
        vis.insert(std::make_pair(cur.first, cur.second + 1));
      }
      if (cur.second - 1 >= 0 && grid[cur.first][cur.second - 1] == 0 &&
          !vis.count(std::make_pair(cur.first, cur.second - 1))) {
        queue.push(std::make_pair(cur.first, cur.second - 1));
        vis.insert(std::make_pair(cur.first, cur.second - 1));
      }
    }
    level++;
  }
}

int ShortestDistance(std::vector<std::vector<int> > & grid) {
  int m = grid.size();
  if (m == 0) return -1;

  int n = grid[0].size();
  std::vector<std::vector<int> > dist(m, std::vector<int>(n, 0));
  std::vector<std::vector<int> > num(m, std::vector<int>(n, 0));
  int cnt = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (grid[i][j] == 1) {
        cnt++;
        BFS(grid, dist, num, i, j, m, n);
      }
    }
  }

  int min = INT_MAX;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (num[i][j] == cnt && min > dist[i][j]) min = dist[i][j];
    }
  }
  return min == INT_MAX ? -1 : min;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > vec(3, std::vector<int>(5, 0));
  vec[0][0] = 1;
  vec[0][4] = 1;
  vec[2][2] = 1;
  vec[0][2] = 2;

  LOG(INFO) << ShortestDistance(vec);
  return 0;
}
