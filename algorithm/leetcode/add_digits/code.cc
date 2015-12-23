// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 11:15:11
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

/*
假设输入的数字是一个5位数字num，则num的各位分别为a、b、c、d、e。
有如下关系：num = a * 10000 + b * 1000 + c * 100 + d * 10 + e
即：num = (a + b + c + d + e) + (a * 9999 + b * 999 + c * 99 + d * 9)
因为 a * 9999 + b * 999 + c * 99 + d * 9 一定可以被9整除，
因此num模除9的结果与 a + b + c + d + e 模除9的结果是一样的。
对数字 a + b + c + d + e 反复执行同类操作，最后的结果就是一个 1-9 的数字加
上一串数字，最左边的数字是 1-9 之间的，右侧的数字永远都是可以被9整除的。
*/
namespace algorithm {
int AddDigits(int num) {
  if (num == 0) return 0;
  if (num % 9 == 0) return 9;
  return num % 9;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
