// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-06 15:24:08
// File  : code.cc
// Brief :
//

// poj 3518

#include "base/public/common_head.h"

namespace algorithm {

std::vector<int> Prime(int n) {
  std::vector<int> rs;
  rs.push_back(1);
  for (int i = 2; i <= n; i++) {
    bool flag = true;
    for (int j = 2; j * j <= i; j ++) {
      if (i % j == 0) {
        flag = false;
        break;
      }
    }
    if (flag) rs.push_back(i);
  }
  return rs;
}

// http://blog.csdn.net/linkrules/article/details/7965556
std::vector<int> PrimeTable(int n) {
  std::vector<int> rs;
  rs.push_back(1);
  for (int i = 2; i <=n; i++) {
    int j = 1;
    while (j < rs.size() && i % rs[j] != 0 && rs[j] * rs[j] <= i) j++;
    if (j >= rs.size() || rs[j] * rs[j] > i) {
      rs.push_back(i);
    }
  }
  return rs;
}

/* 
 * 找质数算法（Sieve of Eratosthenes筛法）
 * 由于一个合数总是可以分解成若干个质数的乘积，
 * 那么如果把质数（最初只知道2是质数）的倍数都去掉，那么剩下的就是质数了。
 * 例如要查找100以内的质数，首先2是质数，把2的倍数去掉；此时3没有被去掉，
 * 可认为是质数，所以把3的倍数去掉；再到5，再到7，7之后呢，
 * 因为8，9，10刚才都被去掉了，而100以内的任意合数肯定都有一个因子小于10
 * （100的开方），所以，去掉，2，3，5，7的倍数后剩下的都是质数了。
 * http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 * http://www.cnblogs.com/guoxiaocong/archive/2005/12/27/305611.html
*/

std::vector<int> Eratosthenes(int n) {
  // 最好用bitset
  std::vector<bool> v(n + 1, true);
  for (int i = 2; i * i <= n; i++) {
    if (v[i]) {
      // Note : can from i^2
      // i * j (j < i是被小于ｊ的素数剔除的)
      for (int j = i; j * i <= n; j++) v[j * i] = false;
    }
  }
  std::vector<int> rs;
  for (int i = 1; i <= n; i++) {
    if (v[i]) rs.push_back(i);
  }
  return rs;
}

// http://blog.csdn.net/morewindows/article/details/7347459
// 每个合数由这种形式唯一确定　x = a * b (a <= b and ａ是小于ｘ的最小的素数)
std::vector<int> EratosthenesOpt(int n) {
  std::vector<bool> v(n, true);
  std::vector<int> rs;
  v[0] = v[1] = false;
  // 这个循环还使用于每个素数都需要剔除起k倍的数
  for (int i = 2; i < n; i++) {
    if (v[i] == true) {
      rs.push_back(i);
    }
    for (int j = 0; j < rs.size(); j++) {
      if (rs[j] * i < n) v[rs[j] * i] = false;
      else break;
      // 2 * 6 = 12, 12剔除， 3 * 6 = 18，18不是在这里剔除的，
      // 是在2 * 9，也就是每个合数，是有起最小的素约数剔除的
      // 18必然在扫描到i= 18前剔除，因为在i=9的时候就执行了
      // 5 * 6 = 3 * 10 所以５*6 需要直接跳过
      if (i % rs[j] == 0) break;
    }
  }
  return rs;
}

/*
 * 目前最快的算法，没有看证明过程
 * http://cr.yp.to/primegen.html 开源代码
 * http://umumble.com/blogs/algorithm/334/
 * Theorem. Let n - natural number, which is not divisible by any perfect square. Then:
 *
 * 1. If n is presented in the form 4k +1, then it is just if and only if, 
 *    when the number of natural solutions of the equation 4x2+y2 = n is odd.
 * 2. If n is presented in the form 6k +1, then it is just if and only if, 
 *     when the number of natural solutions of the equation 3x2+y2 = n is odd.
 * 3. If n is presented in the form of 12k-1, then it just then and only if,
 *    when the number of natural solutions of the equation 3x2−y2 = n for which x > y is odd.
 * The theorem proving can be found in [4].
 *
 * The elementary number theory implies that all pr2+y2], S[3x2+y2], and if x > y, then in S [3x2−y2] as well. 
 * After computing the numbers of cells of the form 6k ± 1, containing odd numbers, 
 * they are prime numbers or divided into squares of primes. 
 *
 * As a final step, we go sequentially through supposedly prime numbers and cross out multiples of their squares.
 * http://blog.csdn.net/turingo/article/details/8161061
 * */
std::vector<int> Atkin(int n) {
  // 最好用bitset
  std::vector<bool>v (n + 1, false);
  v[1] = true; v[2] = true; v[3] = true;
  for (int x = 1; x * x <= n; x++) {
    for (int y = 1; y * y <= n; y++) {
      int t = 4 * x * x + y * y;
      if (t <= n && (t % 12 == 1 || t %12 == 5)) v[t] = !v[t];
      t = 3 * x * x + y * y;
      if (t <= n && t % 12 == 7) v[t] = ! v[t];
      t = 3 * x * x - y * y;
      if (x > y && t <=n && t % 12 ==11) v[t] = !v[t];
    }
  }
  for (int i = 5; i * i <= n; i++) {
    if (v[i]) {
      for (int j = 1; j * i * i <= n; j++) {
        v[j * i * i] = false;
      }
    }
  }
  std::vector<int> rs;
  for (int i = 1; i <= n; i++) {
    if (v[i]) rs.push_back(i);
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  int n = 100;
  LOG(INFO) << JoinVector(Prime(n));
  LOG(INFO) << JoinVector(PrimeTable(n));
  LOG(INFO) << JoinVector(Eratosthenes(n));
  LOG(INFO) << JoinVector(Atkin(n));
  return 0;
}
