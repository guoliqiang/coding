// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-28 02:13:54
// File  : code.cc
// Brief :
/*
 *  Fermat(费尔马)小定理 http://book.51cto.com/art/200812/102434.htm
 * 1. 第一种形式
 * 第一种形式认为如果p是一个素数，且a是一个不能被p整除的整数，那么 。
 * 2. 第二种形式
 * 第二种形式取消了对a的限制条件。如果p是素数，a为整数，那么 。
 * 3.
 * 如果p是素数，a是小于p的正整数，那么a^(p-1) mod p = 1。
 *
 *
 * Miller和Rabin两个人的工作让Fermat素性测试迈出了革命性的一步，建立了传说中的Miller-Rabin素性测试算法。
 * 新的测试基于下面的定理：如果p是素数，x是小于p的正整数，且x^2 mod p = 1，那么要么x=1，要么x=p-1。这是显然的，
 * 因为x^2 mod p = 1相当于p能整除x^2-1，也即p能整除(x+1)(x-1)。由于p是素数，那么只可能是x-1能被p整除(此时x=1)
 * 或x+1能被p整除(此时x=p-1)。
 *
 * http://www.matrix67.com/blog/archives/234
 * http://blog.csdn.net/hanhansoul/article/details/9060203
 * http://www.cnblogs.com/jiaohuang/archive/2010/09/05/1818663.html
 *
 * http://www.cnblogs.com/vongang/archive/2012/03/15/2398626.html
 * */

#include "base/public/common_head.h"

namespace algorithm {

// 快速求解 (a * b) % n
// 例如: b = 1011101 那么a * b mod n = (a * 1000000 mod n + a * 10000 mod n + a * 1000 mod n + a * 100 mod n + a * 1 mod n) mod n 

int Mod(int a, int b, int n) {
  int tmp = 0;
  while (b) {
    if (b & 1) tmp = (tmp + a) % n;
    a = (a * 2) % n;
    b >>= 1;
  }
  return tmp;
}

// 快速求解 (a ^ b) % n
// b =110
// (a^b) % n  = (a^(2^2) % n * a^(2^1) % n) %n
int Mod2(int a, int b, int n) {
  int tmp = 1;
  while (b) {
    if (b & 1) tmp = (tmp * a) % n;
    a = (a * a) % n;
    b >>= 1;
  }
  return tmp;
}
/*
 * 费马小定理：对于素数p和任意整数a，有ap ≡ a(mod p)（同余）。反过来，满足ap ≡ a(mod p)，p也几乎一定是素数。
 *
 * 伪素数：如果n是一个正整数，如果存在和n互素的正整数a满足 an-1 ≡ 1(mod n)，我们说n是基于a的伪素数。如果一个数是伪素数，那么它几乎肯定是素数。
 *
 * Miller-Rabin测试：不断选取不超过n-1的基b(s次)，计算是否每次都有b^(n-1) ≡ 1(mod n)，若每次都成立则n是素数，否则为合数
 * 
 * 二次探测定理：
 * 如果p是素数，则 x^2 ≡ 1(mod p)的解为 x = 1 || x = p - 1;
 * */

}  // namespace algorithm

namespace algorithm {
//
// [a ^(n-1)] % n
// n - 1一定是偶数可以表是为 u * 2^t
//
// ==> a^(n - 1) = (a^u)^(2^t)
//     即a^u平方t次
//
//     a^(n - 1) % n  == (a^u % n)^(2^t)
//
bool Prime(int a, int n) {
  int t = 0;
  int u = n - 1;
  while (u & 1) {
    t++;
    u >>= 1;
  }
  int pre = (int)pow(a, u) % n;
  for (int i = 0; i < t; i++) {
    int cur = (pre * pre) % n;
    if (cur == 1 && pre != 1 && pre != n - 1) {  // 违反Miller和Rabin的工作
      return false;
    }
    pre = cur;
  }
  return pre == 1 ? true : false;
}

bool PrimeWrapper(int n, int s = 50) {
  if (n == 2) return true;
  if (n % 2 == 0 || n == 1) return false;
  for (int i = 0; i < s; i++) {
    // int a = rand() % (n - 1) + 1;
    int a = rand() * (n - 2) / RAND_MAX + 1;
    // rand()只能随机产生[0, RAND_MAX)内的整数
    // 而且这个RAND_MAX只有32768直接%n的话永远也产生不了
    // [RAND-MAX, n)之间的数
    if (Prime(a, n) == false) return false;
  }
  return true;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  int n = 7;
  LOG(INFO) << n << ":" << PrimeWrapper(n);
  n = 10;
  LOG(INFO) << n << ":" << PrimeWrapper(n);
  return 0;
}
