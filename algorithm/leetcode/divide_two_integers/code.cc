// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-23 15:58:29
// File  : code.cc
// Brief :
#include <algorithm>
#include <cmath>
#include "base/public/logging.h"

namespace algorithm {

int divide(int dividend, int divisor) {
  int num = 0;
  // for -2147483648 abs will return -2147483648
  // I dont know why??
  long long dd = fabs((long double)dividend);
  long long dr = fabs((long double)divisor);
  while (dr <= dd) {
    int k = 1;
    while ((dr << k) <= dd && (dr << k) > 0) {
      k++;
      LOG(INFO) << (dr << k);
    }
    k--;
    dd -= (dr << k);
    num += (1 << k);
  }
  return ((dividend ^ divisor) >> 31) ? -num : num;
}

}  // namespace algorithm

namespace twice {
long long int Divi(long long int dividend, long long int divisor) {
  if (divisor > dividend) return 0;
  if (divisor == dividend) return 1;
  long long int num = 1;
  long long int t = divisor;
  while ((t * 2) < dividend) {
      t *= 2;
      num *= 2;
  }
  return num += Divi(dividend - t, divisor);
  
}
class Solution {
public:
    int divide(int dividend, int divisor) {
        // Note: The Solution object is instantiated only once and is reused by each test case.
        bool minus = ((dividend ^ divisor) >> 31) ? true : false;
        int re = Divi(fabs(dividend), fabs(divisor));
        return minus ? -re : re;
    }
};
}  // namespace twice

namespace NB {
class Solution {
public:
    int divide(int dividend, int divisor) {
        long long a = fabs((double)dividend);;
        long long b = fabs((double)divisor);

        long long ret = 0;
        while (a >= b) {
            long long c = b;
            for (int i = 0; a >= c; ++i, c <<= 1) {
                a -= c;
                ret += 1 << i;
            }
        }

        return ((dividend^divisor)>>31) ? (-ret) : (ret);
    }
};
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << "7/3:" << divide(-2147483648, -2147483648);
  LOG(INFO) << "6/3:" << divide(6, 3);
  LOG(INFO) << "6/5:" << divide(6, 5);
  LOG(INFO) << "6/1:" << divide(6, 1);
  LOG(INFO) << "0/1:" << divide(0, 1);
  
  LOG(INFO) << "-6/1:" << divide(-6, 1);
  LOG(INFO) << "6/-2:" << divide(6, -2);
  return 0;
}
