// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-06 17:26:25
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

void Swap(int & a, int & b) {
  a ^= b;
  b ^= a;
  a ^= b;
}

int Reverse(int a) {
  if (a == 0) return 0;
  int d = a < 0 ? 1 : -1;
  while ((a + d) != 0) {
    d += (d > 0 ? 1 : -1);
  }
  return d;
}

int Abs(int a) {
  if (a < 0) return Reverse(a);
  return a;
}

bool OpsiteSign(int a, int b) {
  return (a < 0 && b >0) || (a > 0 && b < 0);
}

bool Zero(int a) {
  return a == 0;
}

int Minus(int a, int b) {
  return a + Reverse(b);
}

int Multiply(int a, int b) {
  if (Zero(a) || Zero(b)) return 0;
  int a1 = Abs(a);
  int b1 = Abs(b);
  if (a1 < b1) Swap(a1, b1);
  int res = 0;
  for (int i = 1; i <= b1; i++)  res += a1;
  if (OpsiteSign(a, b)) return Reverse(res);
  return res;
}

int Divide(int a, int b) {
  if (Zero(b)) return 0x7fffffff;
  if (Zero(a)) return 0;
  int res = -1;
  int a1 = Abs(a);
  int b1 = Abs(b);
  while (a1 >= 0) {
    res++;
    a1 -= b1;
  }
  if (OpsiteSign(a, b)) return Reverse(res);
  return res;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Divide(8, 2);
  LOG(INFO) << Divide(8, -3);
  LOG(INFO) << Multiply(8, -3);
  LOG(INFO) << Minus(8, -3);
  return 0;
}
