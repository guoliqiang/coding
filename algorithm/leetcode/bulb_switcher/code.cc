// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-25 14:31:49
// File  : code.cc
// Brief :

// https://leetcode.com/discuss/75014/math-solution
// Take 18 and 16 as an example: 18 has factor pairs of [1,18], [2,9], [3,6] while 16 as factor pairs of [1,16], [2,8], [4], as you can see, perfect squares will always have a factor pair that contains only one number which makes it perfect squares.
#include "base/public/common_ojhead.h"

namespace NB {

int BulbSwitch(int n) {
  int ans = 0;
  for (int i = 1; i * i <= n; i++) {
    ans++;
  }
  return ans;
}

int BulbSwitch2(int n) {
  return sqrt(n);
}
}  // namespace NB

namespace algorithm {
int BulbSwitch(int n) {
  int size = n / sizeof(int) + 1;
  int * ptr = new int [size];
  memset(ptr, 0, size * sizeof(int));

  for (int i = 1; i <= n; i++) {
    for (int j = i; j <= n; j += i) {
      int idx = j / sizeof(int);
      int offset = j % sizeof(int);
      if (ptr[idx] & (1 << offset)) {
        ptr[idx] &= ~(1 << offset);
      } else {
        ptr[idx] |= (1 << offset);
      }
    }
  }
  int ans = 0;
  for (int i = 0; i < size; i++) {
    while (ptr[i] != 0) {
      ans++;
      ptr[i] &= (ptr[i] - 1);
    }
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << BulbSwitch(9999999);
  LOG(INFO) << NB::BulbSwitch(9999999);
  LOG(INFO) << NB::BulbSwitch2(9999999);
  return 0;
}
