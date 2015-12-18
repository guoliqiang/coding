// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-18 14:14:27
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int RangeBitwiseAnd(int m, int n) {
  int cnt = 0;
  while (m != n) {
    m >>= 1;
    n >>= 1;
    cnt++;
  }
  return (m << cnt);
}

int RangeBitwiseAnd2(int m, int n) {
  int ans = m;
  for (int i = m + 1; i<= n; i++) {
    ans &= i;
  }
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << RangeBitwiseAnd(3, 4) << " " << RangeBitwiseAnd2(3, 4);
  LOG(INFO) << RangeBitwiseAnd(0, 7) << " " << RangeBitwiseAnd2(0, 7);
  LOG(INFO) << RangeBitwiseAnd(11, 20) << " " << RangeBitwiseAnd2(11, 20);
  LOG(INFO) << RangeBitwiseAnd(21, 25) << " " << RangeBitwiseAnd2(21, 25);
  LOG(INFO) << RangeBitwiseAnd(33333, 44444) << " " << RangeBitwiseAnd2(33333, 44444);
  return 0;
}
