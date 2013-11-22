// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 17:23:40
// File  : code.cc
// Brief :

/*
 *  Implement the “paint fill” function that one might see on many image editing programs.
 *  That is, given a screen (represented by a 2 dimensional array of Colors), a point, and a new color,
 *  fill in the surrounding area until you hit a border of that color.
 *  
 *  实现图像处理软件中的“填充”函数，给定一块区域(可以不规则)，一个种子点和一种新颜色， 填充这块区域，
 *  直到到达这块区域的边界(边界是用这种新颜色画的一条封闭曲线)
 * */

/*
 * 图和树的迭代版本BFS是比较容易实现的
 * 用队列即可
 * */
#include "base/public/common_head.h"

namespace algorithm {

void Trace(std::vector<std::vector<int> > & v, int i, int j, int color) {
  if (v[i][j] == color) return;  // 巧妙的避免了使用visited变量
  v[i][j] = color;
  int n = v.size();
  if (i + 1 < n) Trace(v, i + 1, j, color);
  if (i - 1 >= 0) Trace(v, i - 1, j, color);
  if (j + 1 < n) Trace(v, i, j + 1, color);
  if (j - 1 >= 0) Trace(v, i, j - 1, color);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
