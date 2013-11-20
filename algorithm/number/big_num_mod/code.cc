// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-30 01:37:18
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
// 高效求：a^b % c
// Note: (m1 * m2) % n = ((m1 % n) * (m2 % n)) % n
//       m1^m2 % n = ((m1 % n) ^ m2) % n 此题不用这个公式
// http://baike.baidu.com/view/2385246.htm
// 基于快速求幂算法

long long int Mod(long long int a, long long int b, long long int c) {
  a %= c;
  long long int rs = 1;
  while (b) {
    if (b & 1) rs = (rs * a) % c;
    a = (a * a) % c;
    b >>= 1;
  }
  return rs;
}

int Pow(int a, int b) {
  int rs = 1;
  while (b) {
    if (b & 1) rs *= a;
    a *= a;
    b >>= 1;
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Mod(3, 3 , 4) << " " << Pow(3, 3) % 4;
  LOG(INFO) << Mod(5, 3, 7) << " " << Pow(5, 3) % 7;
  return 0;
}
