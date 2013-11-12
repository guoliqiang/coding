// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 16:55:36
// File  : code.cc
// Brief :

/* 
 * Write a method to compute all permutations of a string.
 *
 * 朴素思路的代码见：
 * http://hawstein.com/posts/8.4.html
 * 代码写的比较复杂了
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

void Trace(std::vector<int> & v, int n) {
  if (n == v.size()) {
    LOG(INFO) << JoinVector(v);
  } else {
    for (int i = n; i < v.size(); i++) {
      std::swap(v[n], v[i]);
      Trace(v, n + 1);
      std::swap(v[n], v[i]);
    }
  }
}

void Trace(std::vector<int> v) {
  Trace(v, 0);
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  Trace(v);
  return 0;
}
