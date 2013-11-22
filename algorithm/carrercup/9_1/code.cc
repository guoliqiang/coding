// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 20:37:35
// File  : code.cc
// Brief :

/*
 * 原文：
 * You are given two sorted arrays, A and B, and A has a large enough buffer at the end to hold B. Write a method to merge B into A in sorted order.
 * 译文：
 * A和B是两个有序数组(假设为递增序列)，而且A的长度足以放下A和B中所有的元素， 写一个函数将数组B融入数组A，并使其有序。
 * */

#include "base/public/common_head.h"

namespace algorithm {

// a 末尾有有足够的空间存储merge后的字符串
void Merge(int * a, int m, int * b, int n) {
  int * ptr = a + m + n - 1;
  m--;
  n--;
  while (n >= 0 && m >= 0) {  // bug fix
    if (a[m] > b[n]) *(ptr--) = a[m--];
    else *(ptr--) = b[n--];
  }
  while (n >= 0)  *(ptr--) = b[n--];
}
// merge后 a链接b后整个区域是有序的
// 不使用辅助空间
// 最坏时间复杂度 n*n
// 最好n
// 平均感觉在n*log(n)
void Merge2(int * a, int m, int * b, int n) {
  for (int i = 0; i < m; i++) {
    if (a[i] > b[0]) {  // 只需要和b[0]比较即可
      std::swap(a[i], b[0]);
    }
    // 保证交换后b仍然是有序的
    for (int i = 0; i < n - 1; i++) {
      if (b[i] > b[i + 1]) std::swap(b[i], b[i + 1]);
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int a[] = {7, 9, 0, 0, 0};
  int b[] = {4, 8};
  // Merge(a, 2, b, 2);
  // for (int i = 0; i < 4; i++) LOG(INFO) << a[i];
  Merge2(a, 2, b, 2);
  for (int i = 0; i < 2; i++) LOG(INFO) << a[i] << " " << b[i];
  return 0;
}
