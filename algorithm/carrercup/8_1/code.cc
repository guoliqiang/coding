// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 15:12:53
// File  : code.cc
// Brief :

/*  Write a method to generate the nth Fibonacci number.
 * n = 50时　递归版本就很慢了，另fibonacci数很容易超出int的表示范围
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

int FibonacciR(int n) {
  if (n <= 0) return 0;
  if (n == 1|| n == 2) return 1;
  return FibonacciR(n - 1) + FibonacciR(n - 2);
}

int FibonacciDP(int n) {
  if (n <= 0) return 0;
  if (n == 1 || n == 2) return 1;
  std::vector<int> dp(n, 0);
  dp[0] = 1;
  dp[1] = 1;
  for (int i = 2; i < n; i++) dp[i] = dp[i - 1] + dp[i - 2];
  return dp[n - 1];
}

std::vector<int> Multiply(std::vector<int> & v1, std::vector<int> &v2) {
  std::vector<int> rs(4, 0);
  rs[0] = v1[0] * v2[0] + v1[1] * v2[2]; 
  rs[1] = v1[0] * v2[1] + v1[1] * v2[3];
  rs[2] = v1[2] * v2[0] + v1[3] * v2[2];
  rs[3] = v1[2] * v2[1] + v1[3] * v2[3];
  return rs;
}

int FibonacciBest(int n) {
  if (n <= 0) return 0;
  if (n == 1 || n == 2) return 1;
  std::vector<int> rs(4, 0);
  rs[0] = 1;
  rs[3] = 1;
  std::vector<int> base(4, 0);
  base[0] = base[1] = base[2] = 1;
  n -= 2;
  while (n) {
    if (n % 2) rs = Multiply(rs, base);
    base = Multiply(base, base);
    n >>= 1;
  }
  return rs[0] + rs[1];
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 50;
  LOG(INFO) << FibonacciR(n);
  LOG(INFO) << " " << FibonacciDP(n);
  LOG(INFO) << " " << FibonacciBest(n);
  return 0;
}
