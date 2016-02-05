// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-18 15:41:51
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool IsHappy(int n) {
  std::set<int> loop;
  while (n != 1) {
    loop.insert(n);
    int sum = 0;
    int cur = n;
    while (cur > 0) {
      sum += (cur % 10) * (cur % 10);
      cur /= 10;
    }
    n = sum;
    if (loop.count(n)) return false;
  }
  return true;
}
}  // namespace algorithm

using namespace algorithm;

namespace NB {
class Solution {
 public:
  bool isHappy(int n) {
    int A = nxt(n), B = nxt(nxt(n));
    while (A != 1 && A != B) {
      A = nxt(A);
      B = nxt(B);
      B = nxt(B);
    }
    return A == 1;
  }

 private:
  int nxt(int num) {
    int res = 0;
    while (num) {
      res += (num % 10) * (num % 10);
      num /= 10;
    }
    return res;
  }
};
}  // namespace NB

int main(int argc, char** argv) {
  LOG(INFO) << IsHappy(33);
  return 0;
}
