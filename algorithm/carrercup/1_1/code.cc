// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 16:32:14
// File  : code.cc
// Brief :

/*
 * Implement an algorithm to determine if a string has all unique characters.
 * What if you can not use additional data structures?
 *
 * 题目的意思居然是可以使用基本的数据结构 即数组，
 * 如果一点儿而外资源都不可以使用的话只 可以先排序
 * 在遍历
 *
 * 还有一种考虑问题的方式是将遍历过的元素移动到特定位置，前提是能
 * 确定下当前空间有足够的空间用于存储
 *
 * bool 非0即为真
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {
// only contaion ASCII
bool IsUnique(const std::string & str) {
  char * bs = new char[32];
  memset(bs, 0, 32* sizeof(char));  // bug fixed : memeset的单位是byte数目
  for (int i = 0; i < str.size(); i++) {
    char ch = str[i];
    char * idx = bs + ch / 8;
    int offset = ch % 8;
    if ((((*idx) >> offset) & 1) > 0) {
      delete [] bs;
      return false;
    }
    (*idx) |= (1 << offset);
  }
  delete [] bs;
  return true;
}
}  // namespace algorithm
using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << IsUnique("abcdefhtrw");
  LOG(INFO) << IsUnique("abcdefhtarw");
  return 0;
}
