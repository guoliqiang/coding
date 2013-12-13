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

long long int PowMod(long long int a, long long int b, long long int c) {
  a %= c;
  long long int rs = 1;
  while (b) {
    if (b & 1) rs = (rs * a) % c;
    a = (a * a) % c;
    b >>= 1;
  }
  return rs;
}

long long int MultiMod(long long a, long long b, long long c) {
  int t = a % c;
  int rs = 0;
  while (b) {
    if (b & 1) rs = (rs + t) % c;
    t = (t * 2) % c;
    b >>= 1;
  }
  return rs;
}

// poj 2635
long long Mod(char * str, int k) {
  int rs = 0;
  int len = strlen(str);
  for (int i = 0; i < len; i+= 3) {
    if (i + 1 == len)
      rs = (rs * 10 + (str[i] - '0')) % k;
    else if (i + 2 == len)
      rs = (rs * 100 + (str[i] - '0') * 10 + (str[i + 1] - '0')) % k;
    else
      rs = (rs * 1000 + (str[i] - '0') * 100 + (str[i + 1] - '0') * 10 + (str[i + 2] - '0')) % k;
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
  LOG(INFO) << PowMod(3, 3 , 4) << " " << Pow(3, 3) % 4;
  LOG(INFO) << PowMod(5, 3, 7) << " " << Pow(5, 3) % 7;
  return 0;
}
