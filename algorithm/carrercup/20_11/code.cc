// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-09 17:26:22
// File  : code.cc
// Brief :

/*
 * Imagine you have a square matrix, where each cell is filled with either black or white.
 * Design an algorithm to find the maximum subsquare such that all four borders are filled with black pixels.
 * 注意：只是限定了4个边，并未对矩形内元素的值做限定. 暴力搜索+剪枝
 * 
 * 如果求4个边是1，矩形内是0的最大矩形呢？ 使用类似于最大1矩阵的方法,需要存储
 * 当前位置以上连续的1和0的行数
 *
 * */

#include "base/public/common_head.h"

namespace wrong {

int MaxSub(std::vector<int> & v) {
  int sequence = -1;
  int max = 0;
  for (int i = 0; i < v.size(); i++) {
    if (v[i] == -1) sequence = -1;
    else if (v[i] == 0) {
      if (sequence == -1) sequence = 0;
      else if (sequence > 0) {
        max = std::max(max, (sequence + 1) * v[i - 1]);
        sequence = 0;
      }
    } else if (i > 0 && v[i] == v[i - 1] && sequence != -1) {
      sequence++;
    }
  }
  return max;
}

int MaxSubSquare(std::vector<std::vector<int> > & v) {
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  int max = -1;
  for (int i = m - 1; i >= 1; i--) {
    std::vector<int> foo;
    for (int j = 0; j < n; j++) {
      if (v[i + 1][j] == 0) foo.push_back(-1);
      else {
        int t = i;
        while (t >= 0 && v[t][j] == '0') t--;
        if (t == - 1) foo.push_back(-1);
        else foo.push_back(i - t + 1);
      }
    }
    max = std::max(max, MaxSub(foo));
  }
  return max;
}

}  // namespace wrong

namespace algorithm {
// 暴力方法，先确定左上顶点，然后从size从大到小
// 依次判断
bool IsOk(std::vector<std::vector<int> > & v, int row, int col, int size) {
  for (int i = 0; i < size; i++) {
    if (v[row + i][col] == 1) return false;
    if (v[row + i][col + size] == 1) return false;
    if (v[row][col + i] == 1) return false;
    if (v[row + size][col + i] == 1) return false;
  }
  return true;
}

int Max(std::vector<std::vector<int> > & v) {
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  int max = 0;
  int curcol = 0;
  while (n - curcol > max) {
    for (int i = 0; i < m; i ++) {
      int size = std::max(n - curcol, m - i);
      while (size > max) {
        if (IsOk(v, i, curcol, size)) {
          max = size;
          break;
        }
        size--;
      }
    }
  }
  return max;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(5);
  v.push_back(-1);
  v.push_back(0);
  v.push_back(0);
  v.push_back(4);
  v.push_back(5);
  v.push_back(0);
  v.push_back(0);
  v.push_back(5);
  v.push_back(5);
  v.push_back(5);
  v.push_back(0);
  LOG(INFO) << wrong::MaxSub(v);
  return 0;
}
