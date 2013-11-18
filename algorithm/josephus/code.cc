// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-11 11:35:59
// File  : code.cc
// Brief :

/*
 * 约瑟夫环问题
 * n从0开始考虑问题容易：
 * 即 n = 3 m = 2 输出 1 而不是2.如果结果要求是2，在函数末尾+1即可
 * */

#include "base/public/common_head.h"

namespace algorithm {

// 求k个输出的数是什么
int Josephus(int n, int m, int k) {
  if (k == 1) {
    if (n == 1) return 0;
    else {
      if (m <= n) return m - 1;
      else return m % n - 1;
    }
  }
  return (Josephus(n - 1, m, k - 1) + m) % n;
}

int JosephusLast(int n, int m) {
  int r = 0;
  for (int i = 1; i <= n; i++) {
    r = (r + m) % i;
  }
  return r;
}

// 计算当前删除的元素处于当前串的第几个，从0开始编号
// 当前删除的是第j个，则下一个删除的元素要往前循环移动m个
//
void JosephusSequence(int n, int m) {
  int j = 0;
  for (int i = 0; i < n; i++) {
    j = (j + (m - 1)) % (n - i);
    LOG(INFO) << j << " of " << (n - i);
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  JosephusSequence(5, 2);
  return 0;
  for (int i = 1; i <= 5; i++) {
    LOG(INFO) << Josephus(5, 2, i) + 1;
  }

  return 0;
}
