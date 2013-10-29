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

// 不必关心 x < y
// 如果x < y, 递归一次会交换两个数的值
int GcdSimple(int x, int y) {
  return y ? GcdSimple(y, x%y) : x;
}

// 快速Gcd??
// 感觉收敛速度没有辗转相除快
int kGcd(int x, int y) {
  if (!x) return y;
  if (!y) return x;
  if (!(x & 1) && !(y & 1)) return kGcd(x >> 1, y >> 1) << 1;
  else if (!(x & 1)) return kGcd(x >> 1, y);
  else if (!(y & 1)) return kGcd(x, y >> 1);
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
 *   对于不定整数方程pa+qb=c，若 c mod Gcd(p, q)=0,则该方程存在整数解，否则不存在整数解(左边是gcd的倍数而右边不可能是)。
 *   设c = c' * gcd, 我们先考虑方程  ax + by = gcd, 这样由扩展gcd便可求出一组解 （x', y'), 则（c'x', c'y')就是原方程的一组解，
 *   然后考虑通解：
 *   假设有两组解(x1, y2) ,  (x2, y2), 有  ax1 + by1 == ax2 + by2 = c,   移项得：  a(x1 - x2) == b(y2 - y1),  消去d后有  a'(x1 - x2) == b'(y2 - y1),
 *   此时a' 和 b' 互素， 所以（x1 - x2)一定是b'的倍数， 而(y2 - y1)一定是a'的倍数， 由此可得到通解：
 *   给一组特解(x, y), 通解为(x - kb/gcd, y + ka/gcd)  == (x - kb'  y + ka').
 *
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

// n个数的最大公约数
int nGcd(std::vector<int> v) {
  if (v.size() == 1) return v[0];
  else {
    return Gcd(v[0], nGcd(std::vector<int>(v.begin() + 1, v.end())));
  }
}

// 最小公倍数
int Lcm(int x, int y) {
  return x * y / Gcd(x, y);
}

// n个数的最小公倍数
int nLcm(std::vector<int> v) {
  if (v.size() == 1) return v[0];
  else {
    return Lcm(v[0], nLcm(std::vector<int>(v.begin() + 1, v.end())));
  }
}

/*
 * 模线性方程
 *
 * 已知a, b, n 求x使得  a * x '=' b (% n)  // 其中 ‘=’ 其实是三个横线的恒等号≡，意思是 a * x 与b 关于 n 同余
 * 也就是 (a * x) % n == b % n
 *
 *  a * x '=' b (% n) ==> a * x + n* y = b 所以只有当gcd(a, n) 整除于b的时候上式才会有解（可能有若干个解）,
 *  否则无解
 *  比如：3 * x '=' 6 (% 27)
 *  1, x = 2 (2 + 27, 2 + 27 *2, ...) // 算一组解
 *  2, x = 11 (11 + 27,...)
 *  3, x = 20 (20 + 27, ...)
 *
 *  1) 方程ax '='b (mod n)对于未知量x有解，当且仅当 gcd(a,n)|b (即b%gcd(a,n)==0) 。
 *  2) 方程ax≡b(mod n)或者对模n有d个不同的解，其中d=gcd(a,n), a, n 互素时有唯一解
 *  4) 假设方程ax≡b(mod n)有解，x0是该方程的任意一个解，则该方程对模n恰好有d个不同的解，
 *     分别为：xi = x0 + i(n/d) (i=1,2,……,d-1).
 * */
bool ModDeq(int a, int b, int n, int t) {
  t = Gcd(a, n);
  LOG(INFO) << "gcd:" << t;
  int foo = t;
  if (b % t != 0) {
    LOG(INFO) << "no answer";
    return false;
  } else {
    int x = 0;
    int y = 0;
    exGcd(a, n, x, y);
    t = (x * (b / t)) % n;
    for (int i = 0; i < foo; i++) {
      LOG(INFO) << i << ":" << t + i * n / foo;
    }
    return true;
  }
}

/*
 * 中国剩余定理
 * http://wenku.baidu.com/view/588b38210722192e4536f6a6.html?pn=50
 * 在整数中，若m1 m2 m3两两互素，则同余方程组：
 * x '=' b1(% m1)
 * x '=' b2(% m2)
 * x '=' b3(% m3)
 * 有解，且在模(m1 * m2 * m3) 的意义下唯一
 * 为：
 * M1 * b1 + M2 * b2 + M3* b3
 * 其中 M1为m2 m3的公倍数中最小的模 m1 等于1 的数（一定存在 (m2 * m3) * x '=' 1 (% m1),由前面的结论可知，一定存在）
 * 其中 M2为m1 m3的公倍数中最小的模 m2 等于1 的数
 * 其中 M3为m1 m2的公倍数中最小的模 m3 等于1 的数
 * 
 * 其实结果可以为上值  + k * (m1*m2*m3)
 *
 * 若m1 m2 m3不两两互素，无解，因为 假设m1, m3不互素， 则 gcd(m2 * m1, m3) == 1不成立
 *
 * */

int China(std::vector<int> & m, std::vector<int> &b) {
  int t = 1;
  int rs = 0;
  for (int i = 0; i < m.size(); i++) t *= m[i];
  for (int i = 0; i < m.size(); i++) {
    int tmp = t / m[i];
    int x = 0;
    int y = 0;
    exGcd(tmp, m[i], x, y);
    rs = (rs + tmp * x * b[i]) % t;
  }
  if (rs > 0) return rs;
  return rs + t;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  /*
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
  std::vector<int> v;
  v.push_back(8);
  v.push_back(16);
  v.push_back(2);
  LOG(INFO) << nGcd(v);
  */
  /*
  LOG(INFO) << kGcd(2, 8);
  LOG(INFO) << kGcd(8, 2);
  LOG(INFO) << kGcd(15, 16);
  LOG(INFO) << kGcd(16, 15);
  */
  int t = 0;
  if (ModDeq(27, 6, 3, t)) {
    LOG(INFO) << t;
  }
  std::vector<int> m;
  std::vector<int> b;
  m.push_back(3);
  m.push_back(5);
  m.push_back(7);
  b.push_back(2);
  b.push_back(3);
  b.push_back(2);
  LOG(INFO) << China(m, b);
  
  LOG(INFO) << GcdSimple(2, 8);
  LOG(INFO) << GcdSimple(8, 2);
  LOG(INFO) << GcdSimple(15, 16);
  LOG(INFO) << GcdSimple(16, 15);
  return 0;
}
