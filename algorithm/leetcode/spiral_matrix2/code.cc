// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 15:17:48
// File  : code.cc
// Brief :

/*
螺旋遍历矩阵
Given an integer n, generate a square matrix filled with elements from 1 to n^2 in spiral order.
For example,
Given n = 3,

You should return the following matrix:
[
 [ 1, 2, 3 ],
 [ 8, 9, 4 ],
 [ 7, 6, 5 ]
]

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 6/6 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 28 milli secs
 * Progress: 21/21 test cases passed.
 *
 * */

#include <vector>
#include "base/public/logging.h"
#include "base/public/string_util.h"

namespace algorithm {

void Fill(std::vector<std::vector<int> > & v, int b, int n, int & num) {
  // 注意处理两个边界
  if (n <= 0) return;
  if (n == 1) {
    v[b][b] = num++;
  } else {
    int size = n - 1;
    for (int i = 0; i < n - 1; i++) v[b][b + i] = num++;
    for (int i = 0; i < n - 1; i++) v[b + i][b + size] = num++;
    for (int i = 0; i < n - 1; i++) v[b + size][b + size - i] = num++;
    for (int i = 0; i < n - 1; i++) v[b + size - i][b] = num++;
  }
}

std::vector<std::vector<int> > GenerateMatrix(int n) {
  std::vector<std::vector<int> > rs(n, std::vector<int>(n, 0));
  int k = n;
  int num = 1;
  for (int i = 0; i <= rs.size() / 2; i++) {
    Fill(rs, i, k, num);
    k -= 2;
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
using namespace std;
vector<vector<int> > generateMatrix(int n) {
  std::vector<std::vector<int> > matrix(n, std::vector<int>(n, 0));
  int start = 0;
  int end = n - 1;
  int num = 1;
  while (start < end) {
    for (int i = start; i < end; i++) matrix[start][i] = num++;
    for (int i = start; i < end; i++) matrix[i][end] = num++;
    for (int i = end; i > start; i--) matrix[end][i] = num++;
    for (int i = end; i > start; i--) matrix[i][start] = num++;
    start++;
    end--;
  }
  if (start == end) matrix[start][start] = num;
  return matrix;
}
}  // namespace twice

namespace third {
// spiral 遍历矩阵的模板，可以处理不是方阵的情况
std::vector<std::vector<int> > Generate(int n) {
  std::vector<std::vector<int> > matrix(n, std::vector<int>(n, 0));
  int num = 1;
  int bx  = 0;
  int ex = n - 1;
  int by = 0;
  int ey = n - 1;
  while (bx <= ex && by <= ey) {
    for (int i = by; i < ey; i++) matrix[bx][i] = num++;
    for (int i = bx; i < ex; i++) matrix[i][ey] = num++;
    if (bx == ex) {
      matrix[bx][bx] = num++;
      break;
    }
    if (by == ey) {
      matrix[by][by] = num++;
      break;
    }
    for (int i = ey; i > by; i--) matrix[ex][i] = num++;
    for (int i = ex; i > bx; i--) matrix[i][bx] = num++;
    bx++;
    ex--;
    by++;
    ey--;
  }
  return matrix;
}
}  // namespace third


int main(int argc, char** argv) {
  // std::vector<std::vector<int> > rs = GenerateMatrix(0);
  // std::vector<std::vector<int> > rs = twice::generateMatrix(4);
  std::vector<std::vector<int> > rs = third::Generate(3);
  LOG(INFO) << JoinMatrix(&rs);
  return 0;
}
