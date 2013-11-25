// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 16:12:16
// File  : code.cc
// Brief :

/*
 * Write a function that adds two numbers. You should not use + or any arithmetic operators.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

// 递归版本好写
// a, b 可以是负数
int Add(int a, int b) {
  if (b == 0) return a;
  int t = a ^ b;
  int c = (a & b) << 1;
  return Add(t, c);
}

int Add2(int a, int b) {
  int rs = a ^ b;
  int c = (a & b) << 1;
  while (c) {
    rs ^= c;
    c = (rs & c) << 1;
  }
  return rs;
}
}  // namespace algorithm

namespace NB {
/*
 * 对于这道题目，还有一个非常巧妙的解法。我们知道，数组操作本质上其实是指针操作。 
 * 数组名其实是指向数组首元素地址的指针。比如说整数数组a，a[1]表示的是数组中的第 1个元素，
 * 这是一直以来我们的理解。而编译器看到a[1]，它是怎么去理解的呢？
 *
 * 首先，它会用数组首元素地址，加上偏移量，得到目标数据的地址， 然后再把里面的数据按指针
 * 指向类型的大小取出。所以，当编译器看到a[1]， 它实际上做了下面的事：
 * 假设a指向的地址为0xbfc86d98
 *
 * 得到目标数据地址：0xbfc86d98 + sizeof(int) * 1 = 0xbfc86d9c
 * 取出0xbfc86d9中的数据
 *
 * */
int Add(int a, int b) {
  char * p = reinterpret_cast<char *>(a);  // 因为sizeof(char) = 1
  return (long)(&(p[b]));
}

}  // namespace NB

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Add(-1, 5);
  LOG(INFO) << NB::Add(-1, 5);
  return 0;
}
