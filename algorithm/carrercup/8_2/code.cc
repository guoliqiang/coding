// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 15:41:08
// File  : code.cc
// Brief :

/*
 * Imagine a robot sitting on the upper left hand corner of an NxN grid.
 * The robot can only move in two directions: right and down.
 * How many possible paths are there for the robot?
 * FOLLOW UP
 * Imagine certain squares are “off limits”, such that the robot can not step on them.
 * Design an algorithm to get all possible paths for the robot.
 *
 * 如果从ｎ＊ｎ的左下不跨越对角线走到右上，而且只能向左或向上移动，其不同路径数为catlan数.
 * （只允许往上和往左走是为了保证最后栈为空）
 * 左走相当于进栈，上走相当于出栈, 因为不能越界，始终要求左走的次数大于等于上走的次数.
 *
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {

/*
 * 由于只往下或右走，所以不可能走回去，因此可以省略visited
 * 
 * 方法２: dp但需要保存每一点的所有路径,如果只求有多少条路径
 * 　　　　和最短路径，dp很合适.
 *
 * 如果用纯数学的方法来解这道题目，大概也就是个高中排列组合简单题吧。
 * 机器人从(1, 1)走到(m, n)一定要向下走m-1次，向右走n-1次，不管这过
 * 程中是怎么走的。 因此，一共可能的路径数量就是从总的步数(m-1+n-1)
 * 里取出(m-1)步，作为向下走的步子， 剩余的(n-1)步作为向右走的步子。
 * 路径数等于: C(m -1 + n - 1, m - 1);
 *
 *
 * */

void Trace(std::vector<std::vector<int> > & v,
           std::vector<std::vector<int> > & visited,
           std::vector<std::pair<int, int> > & path,
           int i, int j) {
  int n = v.size();
  visited[i][j] = 1;
  path.push_back(std::make_pair(i, j));
  if (i == n - 1 && j == n - 1) {
    for (int k = 0; k < path.size(); k++)
      std::cout << "[" << path[k].first << "][" << path[k].second << "]";
    std::cout << std::endl;
  } else {
    if (i + 1 < n && visited[i + 1][j] == 0 && v[i + 1][j] == 0) Trace(v, visited, path, i + 1, j);
    if (j + 1 < n && visited[i][j + 1] == 0 && v[i][j + 1] == 0) Trace(v, visited, path, i, j + 1);
  }
  visited[i][j] = 0;
  path.pop_back();
}

void FindPath(std::vector<std::vector<int> > & v) {
  int n = v.size();
  std::vector<std::vector<int> > visited(n, std::vector<int>(n, 0));
  std::vector<std::pair<int, int> > path;
  Trace(v, visited, path, 0, 0);
}

}  // namespace algorithm

namespace algorithm {
// 只求存在多少条路径,因为只能往下和右走，不存在重复走的问题,
// 如果可以随意走，不可以用DP
int DP(std::vector<std::vector<int> > & matrix) {
  int n = matrix.size();
  std::vector<std::vector<int> > dp(n, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) {
    dp[0][i] = 1;
    dp[i][0] = 1;
  }
  for (int i = 1; i < n; i++) {
    for (int j = 1; j < n; j++) {
      dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
    }
  }
  return dp[n - 1][n - 1];
}

// 求存在多少条路径, 可以随意走，但不能往回走
// 可以把所有的路径输出
void Trace(std::vector<std::vector<int> > & matrix, std::vector<std::vector<int> > & visited,
           int x, int y, int & rs) {
  visited[x][y] = 1;
  int n = matrix.size();
  if (x == n - 1 && y == n - 1) rs++;
  else {
    if (x + 1 < n && visited[x + 1][y] == 0) Trace(matrix, visited, x + 1, y, rs);
    if (y + 1 < n && visited[x][y + 1] == 0) Trace(matrix, visited, x, y + 1, rs);
    if (x - 1 >= 0 && visited[x - 1][y] == 0) Trace(matrix, visited, x - 1, y, rs);
    if (y - 1 >= n && visited[x][y - 1] == 0) Trace(matrix, visited, x, y - 1, rs);
  }
  visited[x][y] = 0;
}

int Trace(std::vector<std::vector<int> > & matrix) {
  int rs = 0;
  int n = matrix.size();
  std::vector<std::vector<int> > visited(n, std::vector<int>(n, 0));
  Trace(matrix, visited, 0, 0, rs);
  return rs;
}

}  // namespace algorithm

// 允许往任意方向走，求到左下的最短距离，相当与在一个给定的图上，指定两点
// 求这两点直接的最短路，套用网络流找增广路径的算法. 
namespace algorithm {
// matrix 值为1表示此位置被block
int BFS(std::vector<std::vector<int> > matrix, int b_x, int b_y, int e_x, int e_y) {
  int m = matrix.size();
  int n =  matrix[0].size();
  std::vector<std::vector<int> > path(m, std::vector<int>(n, 0));
  if (matrix[e_x][e_y] == 1 || matrix[b_x][b_y] == 1) return INF;
  std::stack<std::pair<int, int> > stack;
  stack.push(std::make_pair(b_x, e_x));
  matrix[b_x][b_y] = 2;
  path[b_x][b_y] = 1;
  while (!stack.empty()) {
    std::pair<int, int> t = stack.top();
    stack.pop();
    if (t.first == e_x && t.first == e_y) break;
    if (t.first + 1 < m && matrix[t.first + 1][t.second] == 0) {
      matrix[t.first + 1][t.second] = 2;
      stack.push(std::make_pair(t.first + 1, t.second));
      path[t.first + 1][t.second] = path[t.first][t.second] + 1;
    }
    if (t.second + 1 < n && matrix[t.first][t.second + 1] == 0) {
      matrix[t.first][t.second + 1] = 2;
      stack.push(std::make_pair(t.first, t.second + 1));
      path[t.first][t.second + 1] = path[t.first][t.second] + 1;
    }
    if (t.first - 1 >= 0  && matrix[t.first - 1][t.second] == 0) {
      matrix[t.first - 1][t.second] = 2;
      stack.push(std::make_pair(t.first - 1, t.second));
      path[t.first - 1][t.second] = path[t.first][t.second] + 1;
    }
    if (t.second - 1 >= 0 && matrix[t.first][t.second - 1] == 0) {
      matrix[t.first][t.second - 1] = 2;
      stack.push(std::make_pair(t.first, t.second - 1));
      path[t.first][t.second - 1] = path[t.first][t.second] + 1;
    }
  }
  if (matrix[e_x][e_y] != 2) return INF;
  return path[e_x][e_y];
}
}  // namespace algorithm
using namespace algorithm;


int main(int argc, char** argv) {
  int n = 4;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[2][2] = 1;
  FindPath(v);
  return 0;
}
