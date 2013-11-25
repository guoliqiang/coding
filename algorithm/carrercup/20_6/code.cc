// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-09 10:26:36
// File  : code.cc
// Brief :

/*
 * Describe an algorithm to find the largest 1 million numbers in 1 billion numbers. 
 * Assume that the computer memory can hold all one billion numbers.
 *
 * 因为可以全部载入内存，所以可以用快速排序的思想来做，O(n)的时间复杂度.
 *
 * 如果是海量数据，基于一个大小为m的堆，时间复杂度为O(n * log(m))
 * */

#include "base/public/common_head.h"

namespace NB {

/* *期望*时间负载度O(n),(O(n)时间复杂度证明见:http://blog.csdn.net/v_JULY_v/article/details/6370650)
 * 需要改变输入数组元素的位置
 *  
 * 基于快速排序分划的想法
 *
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
