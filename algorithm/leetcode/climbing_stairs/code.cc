// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 21:49:20
// File  : code.cc
// Brief :

/*
You are climbing a stair case. It takes n steps to reach to the top.
Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 10/10 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 45/45 test cases passed.
 *
 * fobonaci 数列
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

std::vector<int> Multiply(std::vector<int> & a, std::vector<int> & b) {
  std::vector<int> rs(4, 0);
  rs[0] = a[0] * b[0] + a[1] * b[2];
  rs[1] = a[0] * b[1] + a[1] * b[3];
  rs[2] = a[2] * b[0] + a[3] * b[2];
  rs[3] = a[2] * b[1] + a[3] * b[3];
  return rs;
}



std::vector<int> Multiply(int n) {
  std::vector<int> rs (4, 0);
  rs[0] = 1;
  rs[2] = 1;
  
  std::vector<int> tmp(4, 1);
  tmp[3] = 0;

  while (n) {
    if (n & 0x1) {
      rs = Multiply(rs, tmp);
    }
    tmp = Multiply(tmp, tmp);
    n >>= 1;
  }
  return rs;
}

int ClimbStairs(int n ) {
  if (n <= 0) return 0;
  if (n == 1) return 1;
  if (n == 2) return 2;
  std::vector<int> foo = Multiply(n - 2);
  return foo[0] * 2 + foo[1];
}


}  // namespace algorithm

using namespace algorithm;

namespace clear {
std::vector<std::vector<int> > Multi(std::vector<std::vector<int> > & x, std::vector<std::vector<int> > & y) {
    std::vector<std::vector<int> > ans(2, std::vector<int>(2, 0));
    ans[0][0] = x[0][0] * y[0][0] + x[0][1] * y[1][0];
    ans[0][1] = x[0][0] * y[0][1] + x[0][1] * y[1][1];
    ans[1][0] = x[1][0] * y[0][0] + x[1][1] * y[1][0];
    ans[1][1] = x[1][0] * y[0][1] + x[1][1] * y[1][1];
    return ans;
}

int climbStairs(int n) {
    std::vector<std::vector<int> > ans(2, std::vector<int>(2, 0));
    std::vector<std::vector<int> > t(2, std::vector<int>(2, 0));
    ans[0][0] = ans[1][1] = 1;
    t[0][0] = t[0][1] = t[1][0] = 1;
    while (n > 0) {
        if (n % 2) ans = Multi(ans, t);
        t = Multi(t, t);
        n /= 2;
    }
    return ans[0][0];
}
}  // namespace clear


int main(int argc, char** argv) {
  LOG(INFO) << ClimbStairs(3);
  LOG(INFO) << ClimbStairs(5);
  LOG(INFO) << ClimbStairs(7);
  LOG(INFO) << ClimbStairs(20);

  return 0;
}
