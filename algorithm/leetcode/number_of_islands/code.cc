// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-18 09:45:08
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void DenoteBFS(std::vector<std::vector<char> > & grid, int i, int j) {
  int m = grid.size();
  int n = grid[0].size();

  std::list<std::pair<int, int> > queue;
  queue.push_back(std::make_pair(i, j));
  while (queue.size() > 0) {
    std::pair<int, int> cur = queue.front();
    queue.pop_front();
    grid[cur.first][cur.second] = '2';
    i = cur.first;
    j = cur.second;
    if (i - 1 > 0 && grid[i - 1][j] == '1') {
      queue.push_back(std::make_pair(i - 1, j));
    }
    if (i + 1 < m && grid[i + 1][j] == '1') {
      queue.push_back(std::make_pair(i + 1, j));
    }
    if (j - 1 > 0 && grid[i][j - 1] == '1') {
      queue.push_back(std::make_pair(i, j - 1));
    }
    if (j + 1 < n && grid[i][j + 1] == '1') {
      queue.push_back(std::make_pair(i, j + 1));
    }
  }
}

void DenoteDFS(std::vector<std::vector<char> > & grid, int i, int j) {
  int m = grid.size();
  int n = grid[0].size();

  grid[i][j] = '2';
  if (i - 1 >= 0 && grid[i - 1][j] == '1') DenoteDFS(grid, i - 1, j);
  if (i + 1 < m && grid[i + 1][j] == '1') DenoteDFS(grid, i + 1, j);
  if (j - 1 >= 0 && grid[i][j - 1] == '1') DenoteDFS(grid, i, j - 1);
  if (j + 1 < n && grid[i][j + 1] == '1') DenoteDFS(grid, i, j + 1);
}
int NumIslands(std::vector<std::vector<char> > grid) {
  int ans = 0;
  int m = grid.size();
  if (m == 0) return ans;
  int n = grid[0].size();

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (grid[i][j] == '1') {
        ans++;
        DenoteDFS(grid, i, j);
      }
    }
  }
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
int Find(std::vector<int> & v, int idx) {
    while (v[idx] != idx) idx = v[idx];
    return idx;
}

void Union(std::vector<int> & v, int x, int y) {
    v[x] = y;
}

int NumIslands(std::vector<std::vector<char> > & grid) {
  int m = grid.size();
  if (m == 0) return 0;
  int n = grid[0].size();
  
  int size = m * n;
  std::vector<int> vec(size, 0);
  for (int i = 0; i < size; i++) vec[i] = i;

  int ans = 0;
  for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
          if (grid[i][j] == '1') {
              ans++;
              if (j - 1 >= 0 && grid[i][j - 1] == '1') {
                  int r1 = Find(vec, i * n + j);
                  int r2 = Find(vec, i * n + j - 1);
                  if (r1 != r2) {
                      ans--;
                      Union(vec, r1, r2);
                  }
              }
              if (i - 1 >= 0 && grid[i - 1][j] == '1') {
                  int r1 = Find(vec, i * n + j);
                  int r2 = Find(vec, (i - 1) * n + j);
                  if (r1 != r2) {
                      ans--;
                      Union(vec, r1, r2);
                  }
              }
          }
      }
  }
  return ans;
}
}  // namepsace twice

int main(int argc, char** argv) {
  std::vector<std::vector<char> > grid;
  grid.push_back(std::vector<char>(2, '1'));
  // grid.push_back(std::vector<char>(3, '0'));
  // grid.push_back(std::vector<char>(3, '1'));
  // grid[1][1] = '1';
  LOG(INFO) << twice::NumIslands(grid);
  return 0;
}
