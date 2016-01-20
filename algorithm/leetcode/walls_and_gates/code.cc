// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 12:23:42
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
using namespace std;

void BFS(std::vector<std::vector<int> > & rooms, int x, int y, int m, int n) {
    std::queue<std::pair<int, int> > queue;
    queue.push(std::make_pair(x, y));
    int level = 0;
    std::set<std::pair<int, int> > vis;
    vis.insert(std::make_pair(x, y));
    while (queue.size()) {
        int size = queue.size();
        for (int i = 0; i < size; i++) {
            int x1 = queue.front().first;
            int y1 = queue.front().second;
            queue.pop();
            if (rooms[x1][y1] > 0) {
                rooms[x1][y1] = std::min(rooms[x1][y1], level);
            }

            if (x1 - 1 >= 0 && rooms[x1 - 1][y1] > 0 && !vis.count(std::make_pair(x1 - 1, y1))) {
              queue.push(std::make_pair(x1 - 1, y1));
              vis.insert(std::make_pair(x1 - 1, y1));
            }
            if (x1 + 1 < m && rooms[x1 + 1][y1] > 0 && !vis.count(std::make_pair(x1 + 1, y1))) {
              queue.push(std::make_pair(x1 + 1, y1));
              vis.insert(std::make_pair(x1 + 1, y1));
            }
            if (y1 - 1 >= 0 && rooms[x1][y1 - 1] > 0 && !vis.count(std::make_pair(x1, y1 - 1))) {
              queue.push(std::make_pair(x1, y1 - 1));
              vis.insert(std::make_pair(x1, y1 - 1));
            }
            if (y1 + 1 < n && rooms[x1][y1 + 1] > 0 && !vis.count(std::make_pair(x1, y1 + 1))) {
              queue.push(std::make_pair(x1, y1 + 1));
              vis.insert(std::make_pair(x1, y1 + 1));
            }
        }
        level++;
    }
}

void WallsAndGates(vector<vector<int> >& rooms) {
  int m = rooms.size();
  if (m == 0) return;
  int n = rooms[0].size();
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (rooms[i][j] == 0) BFS(rooms, i, j, m, n);
    }
  }
}
}  // namespace algorithm

using namespace algorithm;

namespace NB {
void WallsAndGates(vector<vector<int> >& rooms) {
  int m = rooms.size();
  if (m == 0) return;
  int n = rooms[0].size();

  std::queue<std::pair<int, int> > queue;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (rooms[i][j] == 0) queue.push(std::make_pair(i, j));
    }
  }

  int dir[][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  while (queue.size()) {
    std::pair<int, int> cur = queue.front();
    queue.pop();
    for (int i = 0; i < 4; i++) {
      int x = cur.first + dir[i][0];
      int y = cur.second + dir[i][1];
      if (x < 0 || x >= m || y < 0 || y >= n) continue;
      if (rooms[x][y] == INF) {
        rooms[x][y] = rooms[cur.first][cur.second] + 1;
        queue.push(std::make_pair(x, y));
      }
    }
  }
}
}  // namespace NB

int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix(4, std::vector<int>(4, INF));
  matrix[0][1] = -1;
  matrix[0][2] = 0;
  matrix[1][1] = -1;
  matrix[1][3] = -1;
  matrix[2][1] = -1;
  matrix[2][3] = -1;
  matrix[3][0] = -1;
  matrix[3][1] = -1;
  LOG(INFO) << JoinMatrix(&matrix);
  NB::WallsAndGates(matrix);
  LOG(INFO) << JoinMatrix(&matrix);
  return 0;
}
