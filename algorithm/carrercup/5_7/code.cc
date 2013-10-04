// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-04 18:21:26
// File  : code.cc
// Brief :

/*
 * 来自于book的solution
 * */

#include "base/public/common_head.h"

// 思路非常巧妙
namespace NB {
bool Fetch(std::vector<int> & a, int i, int j) {
  return (a[i] & 1 << j) != 0;
}

int FindMissing(std::vector<int> & a, int n) {
  if (n == 32) return 0;
  std::vector<int> odd;
  std::vector<int> even;
  for (int i = 0; i < a.size(); i++) {
    if (Fetch(a, i, n)) odd.push_back(a[i]);
    else even.push_back(a[i]);
  }
  if (odd.size() >= even.size()) {
    return FindMissing(even, n + 1) << 1 | 0;
  } else {
    return FindMissing(odd, n + 1) << 1 | 1;
  }
}

int FindMissing(std::vector<int> & a) {
  return FindMissing(a, 0);
}

}  // namespace NB

namespace algorithm {
/*
 * 常规思路，利用fetch其实是可以构造出a[i]
 * 的然后用sum  或　异或
 *
 * */
}  // namespace algorithm


int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(0);
  foo.push_back(1);
  foo.push_back(2);
  // foo.push_back(3);
  foo.push_back(4);
  foo.push_back(5);
  LOG(INFO) << NB::FindMissing(foo);
  return 0;
}
