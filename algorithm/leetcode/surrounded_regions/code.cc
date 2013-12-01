// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-01 20:59:05
// File  : code.cc
// Brief :

/*
Given a 2D board containing 'X' and 'O', capture all regions surrounded by 'X'.
A region is captured by flipping all 'O's into 'X's in that surrounded region .
For example,
X X X X
X O O X
X X O X
X O X X
After running your function, the board should be:

X X X X
X X X X
X X X X
X O X X
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 16/16 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 52 milli secs
 * Progress: 56/56 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

bool Next(std::vector<std::vector<char> > & v,
          std::set<std::pair<int, int> > & bad,
          int * o_i, int * o_j) {
  int m = v.size();
  int n = (m == 0) ? m : v[0].size();
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (v[i][j] == 'O' && !bad.count(std::make_pair(i, j))) {
        *o_i = i;
        *o_j = j;
        return true;
      }
    }
  }
  return false;
}

bool NextNew(std::vector<std::vector<char> > & v,
             int * o_i, int * o_j) {
  int m = v.size();
  int n = (m == 0) ? m : v[0].size();
  for (int i = 0; i < m; i++) {
    if (v[i][0] == 'O') {
      *o_i = i;
      *o_j = 0;
      return true;
    }
    if (v[i][n - 1] == 'O') {
      *o_i = i;
      *o_j = n - 1;
      return true;
    }
  }
  for (int i = 0; i < n; i++) {
    if (v[0][i] == 'O') {
      *o_i = 0;
      *o_j = i;
      return true;
    }
    if (v[m - 1][i] == 'O') {
      *o_i = m - 1;
      *o_j = i;
      return true;
    }

  }
  return false;
}

void BadRegionNew(std::vector<std::vector<char> > & v, int i_i, int i_j) {
  int m = v.size();
  int n = (m == 0) ? m : v[0].size();
  std::queue<std::pair<int, int> > queue;
  queue.push(std::make_pair(i_i, i_j));
  
  while (!queue.empty()) {
    std::pair<int, int> t= queue.front();
    queue.pop();
    int i = t.first;
    int j = t.second;
    v[i][j] = '+';
    if (i + 1 < m  && v[i + 1][j] == 'O') {
      queue.push(std::make_pair(i + 1, j));
      v[i + 1][j] = '+';  // 必须在这里改，否则循环的次数会增加很多
    }
    if (i - 1 >= 0 && v[i - 1][j] == 'O') {
      queue.push(std::make_pair(i - 1, j));
      v[i - 1][j] = '+';
    }
    if (j - 1 >= 0 && v[i][j - 1] == 'O') {
      queue.push(std::make_pair(i, j - 1));
      v[i][j - 1] = '+';
    }
    if (j + 1 < n  && v[i][j + 1] == 'O') {
      queue.push(std::make_pair(i, j + 1));
      v[i][j + 1] = '+';
    }
  }

}

bool BadRegionN(std::vector<std::vector<char> > & v, int i_i, int i_j,
               std::set<std::pair<int, int> > & bad,
               std::vector<std::vector<bool> > & visited) {
  int m = v.size();
  int n = (m == 0) ? m : v[0].size();
  std::queue<std::pair<int, int> > queue;
  queue.push(std::make_pair(i_i, i_j));
  
  while (!queue.empty()) {
    std::pair<int, int> t= queue.front();
    queue.pop();
    int i = t.first;
    int j = t.second;
    bad.insert(std::make_pair(i, j));
    visited[i][j] = 1;
    if (i + 1 == m || i - 1 == -1 || j + 1 == n || j - 1 == -1) return true;
    if (v[i + 1][j] == 'O' && !visited[i + 1][j]) queue.push(std::make_pair(i + 1, j));
    if (v[i - 1][j] == 'O' && !visited[i - 1][j]) queue.push(std::make_pair(i - 1, j));
    if (v[i][j - 1] == 'O' && !visited[i][j - 1]) queue.push(std::make_pair(i, j - 1));
    if (v[i][j + 1] == 'O' && !visited[i][j + 1]) queue.push(std::make_pair(i, j + 1));
  }
  return false;
}

bool BadRegion(std::vector<std::vector<char> > & v, int i, int j,
               std::set<std::pair<int, int> > & bad,
               std::vector<std::vector<bool> > & visited) {
  int m = v.size();
  int n = (m == 0) ? m : v[0].size();
  bad.insert(std::make_pair(i, j));
  visited[i][j] = 1;
  if (i + 1 == m || i - 1 == -1 || j + 1 == n || j - 1 == -1) return true;
  
  if (v[i + 1][j] == 'O' && !visited[i + 1][j] && BadRegion(v, i + 1, j, bad, visited)) return true;
  if (v[i - 1][j] == 'O' && !visited[i - 1][j] && BadRegion(v, i - 1, j, bad, visited)) return true;
  if (v[i][j - 1] == 'O' && !visited[i][j - 1] && BadRegion(v, i, j - 1, bad, visited)) return true;
  if (v[i][j + 1] == 'O' && !visited[i][j + 1] && BadRegion(v, i, j + 1, bad, visited)) return true;
  
  return false;
}

void SolveNew(std::vector<std::vector<char> > & v) {
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  int i = 0;
  int j = 0;
  while (NextNew(v, &i, &j)) {
    BadRegionNew(v, i, j);
  }
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      if (v[i][j] == 'O') v[i][j] = 'X';
      if (v[i][j] == '+') v[i][j] = 'O';
    }
  }
}

void Solve(std::vector<std::vector<char> > & v) {
  std::set<std::pair<int, int> > bad;
  std::set<std::pair<int, int> > foo;
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  int i = 0;
  int j = 0;
  while (Next(v, bad, &i, &j)) {
    foo.clear();
    std::vector<std::vector<bool> > visited(m, std::vector<bool>(n, false));
    if (BadRegionN(v, i, j, foo, visited)) {
      bad.insert(foo.begin(), foo.end());
    } else {
      for (std::set<std::pair<int, int> >::iterator k = foo.begin();
           k != foo.end(); k ++) {
        v[k->first][k->second] = 'X';
      }
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<char> > v(4, std::vector<char>(4, 'X'));
  v[1][1] = 'O';
  v[2][2] = 'O';
  v[3][1] = 'O';
  LOG(INFO) << JoinMatrix(&v);
  SolveNew(v);
  HERE(INFO);
  LOG(INFO) << JoinMatrix(&v);
  return 0;
}
