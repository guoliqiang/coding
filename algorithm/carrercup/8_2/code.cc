// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 15:41:08
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

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

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 4;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[2][2] = 1;
  FindPath(v);
  return 0;
}
