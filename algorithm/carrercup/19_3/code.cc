// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 11:05:58
// File  : code.cc
// Brief :

/*
 * Write an algorithm which computes the number of trailing zeros in n factorial.
 * */

#include "base/public/common_head.h"

namespace algorithm {

int ZeroNum(int n) {
  int rs = 0;
  for (int i = 1; i <= n; i++) {
    int t = i;
    while (t % 5 == 0) {
      rs++;
      t /= 5;
    }
  }
  return rs;
}

}  // namespace algorithm

namespace NB {
/*
 * 开始时 1 ～ n中含有5的数字可以写成
 * 5 * 1
 * 5 * 2
 * 5 * 3
 * 5 * 4
 * 5 * 5
 * 5 * 6
 * .
 * .
 * .
 *  n / 5 即为 1 ～ n中含有1个5的个数，然后 递归
 * */
int ZeroNum(int n) {
  int rs = 0;
  while ((n /= 5) > 0) {
    rs += n;
  }
  return rs;
}

int ZeroNum2(int n) {
  if (n < 5) return 0;
  return n / 5 + ZeroNum2(n / 5);
}

// 自己常写的形式
int ZeroNum3(int n) {
  int rs = 0;
  while (n) {
    n /= 5;
    rs += n;
  }
  return rs;
}

}  // namespace NB

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << ZeroNum(100) << " " << NB::ZeroNum(100) << " " << NB::ZeroNum2(100) << " " << NB::ZeroNum3(100);
  return 0;
}
