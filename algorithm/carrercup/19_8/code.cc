// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 13:42:21
// File  : code.cc
// Brief :

/*
 * 这道题目和19.2是一个思路。如果只需要查询一次， 那就直接做；如果要多次查询，而且
 * 要查询各种不同的单词，那就先预处理一遍， 接下来就只需要用O(1)的时间进行查询。
 *
 * 只查询一次
 *
 * 遍历这本书的每个单词，计算给定单词出现的次数 或是只统计前四分之一本书某个单词出现的
 * 次数，然后乘以4。这样能计算出一个大概值。 当然，单词均匀分布这个假设太强了，一般是不成立的。
 *
 * 多次查询
 *
 * 如果我们要对一本书进行多次的查询，就可以遍历一次这本书的单词， 把它出现的次数
 * 存入哈希表中。查询的时候即可用O(1)的时间完成。
 * */

#include "base/public/common_head.h"

namespace algorithm {
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
