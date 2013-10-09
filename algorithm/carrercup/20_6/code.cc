// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-09 10:26:36
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace NB {

/* *期望*时间负载度O(n),需要改变输入数组元素的位置
 * 
 * 基于快速排序分划的想法
 * */
void TopMinK(std::vector<int> &v, int b, int e, int k) {
  if (e - b + 1 <= k) return;
  int t = b - 1;  // Note: bug fixed, wrong set t = -1
  for (int i = b; i < e; i++) {
    if (v[i] <= v[e]) std::swap(v[++t], v[i]);
  }
  std::swap(v[++t], v[e]);
  if (t -b + 1 > k) {
    TopMinK(v, b, t - 1, k);
  } else if (t - b + 1 < k) {
    TopMinK(v, t + 1, e, k - (t - b + 1));
  } else {
    return;
  }
}

void TopMinK(std::vector<int> & v, int k) {
  if (k <= 0) return;
  TopMinK(v, 0, v.size() - 1, k);
}

}// namespace NB


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(5);
  v.push_back(15);
  v.push_back(4);
  v.push_back(6);
  v.push_back(3);
  v.push_back(33);
  NB::TopMinK(v, 1);
  LOG(INFO) << JoinVector(v);
  return 0;
}
