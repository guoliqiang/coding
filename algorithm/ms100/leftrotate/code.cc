// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-29 23:30:11
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

void SolveSimple(std::vector<int> & v, int k) {
  k = k % v.size();
  int n = v.size();
  if (k == 0 || n == 0) return;
  for (int i = 0; i < k; i++) {
    int t = v[n - 1];
    for (int j = n - 1; j > 0; j--) {
      v[j] = v[j - 1];
    }
    v[0] = t;
  }
}

void Solve(std::vector<int> & v, int k) {
  k = k % v.size();
  if (k == 0) return;
  int b = 0;
  int e = v.size() - 1;
  // reverse [0, v.size())
  while (b < e) std::swap(v[b++], v[e--]);
  b = 0;
  e = k - 1;
  // reverse [0, k)
  while (b < e) std::swap(v[b++], v[e--]);
  b = k;
  e = v.size() - 1;
  // reverse [k, v.size())
  while (b < e) std::swap(v[b++], v[e--]);
}

int Gcd(int x, int y) {
  if (x == 0) return y;
  if (y == 0) return x;
  return Gcd(y, x % y);
}

void SolveBest(std::vector<int> & v, int k) {
  k = k % v.size();
  if (k == 0) return;
  int n = v.size();
  int c = Gcd(v.size(), k);
  for (int i = 0; i < c; i++) {
    int j = i;
    int cur = v[j];
    do {
      j = (j + k) % n;
      int t = v[j];
      v[j] = cur;
      cur = t;
    } while (j != i);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  // SolveBest(v, 3);
  // Solve(v, 3);
  SolveSimple(v, 3);
  LOG(INFO) << JoinVector(v);
  return 0;
}
