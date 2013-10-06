// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 17:23:40
// File  : code.cc
// Brief :

/*
 * 图和树的迭代版本BFS是比较容易实现的
 * 用队列即可
 * */
#include "base/public/common_head.h"

namespace algorithm {

void Trace(std::vector<std::vector<int> > & v, int i, int j, int color) {
  if (v[i][j] == color) return;
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
