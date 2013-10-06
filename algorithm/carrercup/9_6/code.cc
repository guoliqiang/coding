// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 22:24:13
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

bool Find(std::vector<std::vector<int> > & v, int i, int j, int k) {
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  if (i < 0 || i >= m || j < 0 || j >= n) return false;
  if (v[i][j] == k) return true;
  if (v[i][j] < k) return Find(v, i + 1, j, k);
  else return Find(v, i, j - 1, k);
}

bool Find(std::vector<std::vector<int> > & v, int k) {
  if (v.size() == 0) return false;
  int i = 0;
  int j = v[0].size() - 1;
  return Find(v, i, j, k);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
