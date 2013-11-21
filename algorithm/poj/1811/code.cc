// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-20 18:31:51
// File  : code.cc
// Brief :

/*
 * Accepted  140K 1360MS
 *
 * */
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 10000;
LL factor[MAXN];
int tail = 0;

LL Gcd(LL x, LL y) {
  if (!x) return y;
  if (!y) return x;
  return Gcd(y, x % y);
}


LL MulMod(LL a, LL b, LL n) {
  LL rs = 0;
  LL foo = a % n;
  while (b) {
    if (b & 1) rs = (rs + foo) % n;
    foo = (foo + foo) % n;
    b >>= 1;
  }
  return rs;
}

LL PowMod(LL a, LL b, LL n) {
  LL rs = 1;
  LL foo = a % n;
  while (b) {
    if (b & 1) rs = MulMod(foo, rs, n);
    foo  = MulMod(foo, foo, n);
    b >>= 1;
  }
  return rs;
}

// is n is prime?
bool MR(LL a, LL n) {
  if (n == 2) return true;
  if (n % 2 == 0 || n == 1) return false;
  LL t = 0;
  LL u = n - 1;
  while ((u & 1) == 0) {
    t++;
    u >>= 1;
  }
  LL pre = PowMod(a, u, n);
  for (int i = 0; i < t; i++) {
    LL cur = MulMod(pre, pre, n);
    if (cur == 1 && pre != 1 && pre != n - 1) return false;  // 判断是不是违反MR的工作
    pre = cur;
  }
  return pre == 1;  // 判断是不是满足费马定理
}

// 不确定算法，从理论上讲可能存在将一个合数
// 判断成素数的情况，其出错概率为: 4^(-s)
bool Fermat(LL n, int s = 50) {
  for (int i = 0; i < s; i++) {  // 随机选取s个小于n的数进行测试
    LL a = rand() % (n - 1) + 1;
    if (MR(a, n) == false) return false;
  }
  return true;
}

// 大整数分解的快速算法，它可以在O(sqrt(p))的时间复杂度内找到n的一个小因子p,
// 但是对于一个因子很少、因子值很大的大整数n来说，Pollard rho算法的效率仍然不是很好.
// 一个60位的数，计算其一个因子的平均时间为12天，所以RSA目前还是比较安全的
// 加减法计算需要cpu 1～4个周期
// 乘法需要cpu 2 ～ 8个周期
// 除法需要从少于8周期到多于60周期不等
//
// http://blog.sina.com.cn/s/blog_86a9d97201015cj7.html
// http://www.cnblogs.com/jackiesteed/articles/2019910.html
// http://book.51cto.com/art/200812/102577.htm
// http://book.51cto.com/art/200804/70903.htm
LL PollarRho(LL n, LL c) {
  LL i = 1;
  LL x1 = rand() % n;
  LL x2 = x1;
  LL k = 2;
  while (true) {
    i++;
    x1 = (MulMod(x1, x1, n) + c) % n;  // 原理找到两个合适的数x1, x2, n不能整除x2 - x1, 然后计算gcd(x2 - x1, n)
                                       // 其结果如果不是1或n的话，其一定是n的一个因子
    LL d = x2 - x1 < 0 ? Gcd(x1 - x2, n): Gcd(x2 - x1, n);
    if (d != 1 && d != n) return d; // 此时d一定是n的一个因数
    if (x2 == x1) return n;  // 重复，说明这次查找是失败的
    if (i == k) {
      x2 = x1;
      k += k;
    }
  }
}

// PollarRho需配合Fermat一起使用
void Find(LL n) {
  if (Fermat(n)) {
    factor[tail++] = n;  // 判断n是不是一个质数
  } else {
    LL p = n;
    while (p == n) {
      p = PollarRho(p, rand() % (n - 1) + 1);
    }
    Find(p);  // p是n个一个因子但可能不是质因子
    Find(n / p);
  }
}

void Read() {
  srand(time(NULL));
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int i = 0; i < c; i++) {
    tail = 0;
    LL n = 0;
    scanf("%lld", &n);
    getchar();
    if (Fermat(n)) printf("Prime\n");
    else {
      Find(n);
      LL min = n;
      for (int i = 0; i < tail; i++) min = std::min(min, factor[i]);
      printf("%lld\n", min);
    }
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
