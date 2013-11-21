// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-26 03:04:29
// File  : code.cc
// Brief :
//
// http://wenku.baidu.com/view/b881520b4a7302768e993978.html
// 对正整数n，欧拉函数是少于或等于n的数中与n互质的数的数目。
//
// 对于给定的一个素数 p ， φ(p) = p -1。则对于正整数 n = p^k , φ(n) = p^k - p^k - 1
// 证明：小于 p^k 的正整数个数为 p^k - 1个，其中和p^k 不互质的正整数有{p * 1,p * 2,...,p * (p^(k - 1) -1)}
// 共计 p^(k - 1) - 1 个  所以 φ(n) = (p^k - 1) - (p^(k - 1) - 1) = p^k - p^(k - 1) = p^k * (1 - 1/p)
//
// 假设 p, q是两个互质的正整数，则 p * q 的欧拉函数为 :
// φ(p * q) = φ(p) * φ(q) ， gcd(p, q) = 1 。
//
// 任意一个整数 n 都可以表示为其素因子的乘积为：
// 由于 n = Multi(pi^ki) (任意数都可以表示成质因数积的形式)
// φ(n) = [p1^k1 *( 1 - 1/p1)] * [p2^k2 * (1 - 1 / p2)] * ...
//      = n * (1 - 1 / p1) * (1 - 1 / p2) * (1 - 1 / p3) * ...
//
//欧拉函数的表达式为
// N*(1-1/f_1)*(1-1/f_2)*(1-1/f_3)....依次类推, 其中f_1, f_2, f_3等是N的不相同的质因子
// 首先介绍这种递推关系, 假设数N有m个不相同的质因子f_1, f_2,f_3....f_m. 那么数(N/f_1)有多少个不同的质因子呢? 
// 分成两种情况来考虑, 
// 1. N只包含一个f_1因子, 那么N/f_1有m-1个因子f_2,f_3,...,f_m. 我们考察N/f_1和N的欧拉函数形式E(N) = N*(1-1/f_1)*(1-1/f_2)*(1-1/f_3)*...*(1-1/f_m) 
// E(N/f_1) =  N/f_1*(1-1/f_2)*(1-1/f_3/f_1)*(1-1/f_2)*(1-1/f_3)*...*(1-1/f_m)
// E(N/f_1) = N/f_1*(1-1/f_2)*(1-1/f_3)*...*(1-1/f_m). 把(1-1/f_1)化为(f_1 - 1)/f_1 则可以显然看到E(N) = (f_1 - 1)*E(N/f_1). 
// 
// 第二种情况, N包含一个以上的f_1因子, 那么N/f_1包含了与N相同的质因子个数且此时两者的欧拉函数分别记为
// E(N) = N*(1-1/f_1)*(1-1/f_2)*(1-1/f_3)*...*(1-1/f_m)
// E(N/f_1) = N/f_1*(1-1/f_1)*(1-1/f_2)*(1-1/f_3)*...*(1-1/f_m)
// 这个递推关系更明显了E(N) = (f_1)*E(N/f_1).
//
//
// 如果a为n的素因子：
// if((n/a)%a==0) eular(n)=eular(n/a)*a
// if((n/a)%a!=0) eular(n)=eular(n/a)*(a-1)
//

#include "base/public/common_head.h"

namespace algorithm {

std::vector<int> Eular(int n) {
  std::vector<bool> flag(n, true);
  flag[0] = flag[1] = false;
  std::vector<int> prime;
  std::vector<int> eular(n, 0);
  for (int i = 2; i < n; i++) {
    if (flag[i] == true) {
      prime.push_back(i);
      eular[i] = i - 1;
    }
    for (int j = 0; j < prime.size(); j++) {
      if (prime[j] * i >= n) break;
      flag[prime[j] * i] = false;
      if (i % prime[j] == 0) {
        eular[i * prime[j]] = eular[i] * prime[j];
        break;
      } else {
        eular[i * prime[j]] = eular[i] * (prime[j] - 1);
      }
    }
  }
  return eular;
}

}  // namespace algorithm

namespace algorithm {
const int MAX = 1000;
int N = 10;
int phi[MAX] = {0};

/*
 * 若a, b为素数
 * phi(b) = b - 1;
 * phi(b^2) = b * (b - 1)
 * phi(b^3) = b^2 * (b - 1)
 *
 * phi(a^3*b^3) = a^2 * (a - 1) * b^2 * (b - 1)
 *
 * */

// 批量计算欧拉函数
void BestEular() {
  memset(phi, 0, sizeof(phi));
  for (int i = 1; i <= N; i++) {
    phi[i] = i;
  }
  // 偶数中只有2是素数
  for (int i = 2; i <= N; i += 2) {
    phi[i] /= 2;
    // really: phi[i] = phi[i] / 2 * (2 - 1)
  }
  for (int i = 3; i <= N; i += 2) { // 剩余偶数肯定不是素数
    if (phi[i] == i) {  // 素数
      for (int j = i; j <= N; j += i) {
        phi[j] = phi[j] / i * (i - 1);
      }
    }
  }
}

// 试除法
// 效率低，但好写
int SingleEular(int n) {
  int rs = n;
  for (int i = 2; i < (int) sqrt(n * 1.0) + 1; i++) {
    if (n % i == 0) {
      rs = rs / i * (i - 1);
      while (n % i == 0) n /= i;  // NB : 可以保证i一定是素数
    }
  }
  // 处理n为素数的情况
  if (n > 1) rs = rs / n * (n - 1);
  return rs;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 10;
  std::vector<int> rs = Eular(n);
  for (int i = 0; i < 10; i++) {
    LOG(INFO) << i << ":" << rs[i];
  }
  HERE(INFO);
  N = 10;
  BestEular();
  for (int i = 1; i<= N; i++) {
    LOG(INFO) << i << ":" << phi[i];
  }
  return 0;
}
