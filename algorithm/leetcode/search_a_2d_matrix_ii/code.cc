// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 16:17:26
// File  : code.cc
// Brief :

/*
Write an efficient algorithm that searches for a value in an m x n matrix. This matrix has the following properties:

Integers in each row are sorted from left to right.
The first integer of each row is greater than the last integer of the previous row.
For example,

Consider the following matrix:

[
  [1,   3,  5,  7],
  [10, 11, 16, 20],
  [23, 30, 34, 50]
]

Given target = 3, return true.

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 41/41 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 72 milli secs
 * Progress: 134/134 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

bool SearchMatrix(std::vector<std::vector<int> > & v, int t) {
  int b = 0;
  int m = v.size();
  int n = (m == 0 ? 0 : v[0].size());

  int e = m * n;
  while (b < e) {
    int mid = b + (e - b) / 2;
    int i = mid / n;
    int j = mid % n;
    if (v[i][j] == t) return true;
    else {
      if (v[i][j] < t) b = mid + 1;
      else e = mid;
    }
  }
  return false;
}


}  // namespace algorithm

using namespace algorithm;

// 杨氏矩阵
// 之前关于杨氏矩阵的理解有出入，杨氏矩阵可以按二叉树进行搜索
// 但不能显示的画出这颗二叉树
// [
//   [1,   3,  5,  7],
//   [10, 11, 16, 20],
//   [23, 30, 34, 50]
// ]
//
// 杨氏矩阵的描述：
// １. 每行从小到大排列，每列从小到大排列
// ２．Integers in each row are sorted from left to right.
// 　　The first integer of each row is greater than the last integer of the previous row.
namespace twice {
bool SearchMatrix(std::vector<std::vector<int> > &matrix, int target) {
  int m = matrix.size();
  if (m == 0) return false;
  int n = matrix[0].size();
  int x = 0;
  int y = n - 1;
  while (matrix[x][y] != target) {
    if (matrix[x][y] < target) x++;
    else y--;
    if (x >= m || y < 0) return false;
  }
  return true;
}

}

int main(int argc, char** argv) {
  std::vector<std::vector<int> > v (1, std::vector<int> (4, 0));
  v[0][0] = 0;
  v[0][1] = 1;
  v[0][2] = 2;
  v[0][3] = 3;
  LOG(INFO) << SearchMatrix(v, 0);
  LOG(INFO) << SearchMatrix(v, 2);
  LOG(INFO) << SearchMatrix(v, 3);
  LOG(INFO) << SearchMatrix(v, -1);
  LOG(INFO) << SearchMatrix(v, 5);
  return 0;
}
