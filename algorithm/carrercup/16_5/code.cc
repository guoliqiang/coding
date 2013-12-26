// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-07 14:55:09
// File  : code.cc
// Brief :
// http://zh.wikipedia.org/wiki/%E5%AD%97%E8%8A%82%E5%BA%8F
// 解释的很清楚

// “endian”一词来源于乔纳森·斯威夫特的小说格列佛游记。小说中，小人国为水煮蛋该从大的一
// 端（Big-End）剥开还是小的一端（Little-End）剥开而争论，争论的双方分别被称为
// Big-endians和Little-endians。
//
// 可以理解为从内存底地址往高低值走，先遇到的是数据的高位（大端）还是底位（小端）

#include "base/public/common_head.h"


namespace algorithm {

const int big_endian = 0;
const int little_endian = 1;

// 低位地址存储的是数字的高位，是大端
// 低位地址存储的是数字的低位，是小端
int BigLittleEndian() {
  short int t = 0x0001;
  char * p = reinterpret_cast<char *>(&t);  // p是t的低位地址
  return *p ? little_endian : big_endian;
}

std::string BigLittle() {
  if (BigLittleEndian() == big_endian) return "BIG";
  return "LITTLE";
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << sizeof(int) << " " << sizeof(long) << " " << sizeof(short);
  LOG(INFO) << BigLittle();
  return 0;
}
