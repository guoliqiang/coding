// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-27 18:44:45
// File  : code.cc
// Brief : http://www.cppblog.com/superKiki/archive/2011/03/20/116785.html

#include "base/public/common_head.h"

namespace algorithm {
// 辗转相除
int Gcd(int x, int y) {
  if (!x || !y) return x > y ? x : y;
  while (y) {
    int t = y;
    y = x % y;
    x = t;
  }
  return x;
}

int Gcd2(int x, int y) {
  if (!x) return y;
  if (!y) return x;
  return Gcd2(y, x % y);
}

// 快速Gcd??
// 感觉收敛速度没有辗转相除快
int Kgcd(int x, int y) {
  if (!x) return y;
  if (!y) return x;
  if (!(x & 1) && !(y & 1)) return Kgcd(x >> 1, y >> 1) << 1;
  else if (!(x & 1)) return Kgcd(x >> 1, y);
  else if (!(y & 1)) return Kgcd(x, y >> 1);
  else return (std::abs(x - y), std::min(x, y));
}
// 扩展gcd: 欧几里德算法
// 对已任意的整数a, b 一定存在整数x , y 使得:
// a * x + b * y = gcd(a, b);
//
// 证明：
// 令 a = m1 * gcd(a, b)  b = m2 * gcd(a, b)
// m1 * gcd(a, b) * x + m2 * gcd(a, b) * y = gcd(a, b)
// ==>  m1 * x + m2 * y = 1
// ==> 即寻找整数 x y 使上式成立， 思考一下会得出答案

/*
 *   a * x + b * y = gcd
 *   a' = b  b' = a % b  = a - a / b * b
 *   
 *   若已经计算出a' * x' + b' * y' = gcd(a', b') (其中gcd(a', b') === gcd(a, b))
 *   b * x' + (a - a / b * b) * y' = gcd
 *   a * y' + b * (x' - a / b) * y' = gcd
 *   => x = y'  y = x' - a / b * y'
 *
 *   补充：关于使用扩展欧几里德算法解决不定方程的办法
 *   对于不定整数方程pa+qb=c，若 c mod Gcd(p, q)=0,则该方程存在整数解，否则不存在整数解。
 *
 * */
int exGcd(int a, int b, int & x, int &y) {
  if (!b) {
    x = 1;
    y = 0;  // must
    return a;
  } else {
    int rs = exGcd(b, a % b, x, y);
    int t = x;
    x = y;
    y = t - a / b * y;
    return rs;
  }
}


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Gcd(15, 16);
  LOG(INFO) << Gcd(16, 15);
  LOG(INFO) << Gcd(8, 2);
  LOG(INFO) << Gcd(2, 8);
  LOG(INFO) << Gcd2(2, 8);
  LOG(INFO) << Gcd2(15, 9);
  int x = 0;
  int y = 0;
  exGcd(15, 9, x, y);
  LOG(INFO) << x << "*15 + " << y << "*9";
  return 0;
}
