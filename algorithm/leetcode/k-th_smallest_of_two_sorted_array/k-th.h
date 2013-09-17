// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-17 16:01:51
// File  : k-th.h
// Brief :

#ifndef  __KTH_H_
#define  __KTH_H_

#include <vector>
#include "base/public/logging.h"

namespace algorithm {

// a and b is sorted increasely.
template <typename type>
type FindKthSmallest(type * a, int m, type * b, int n, int k,
                     type MAX, type MIN) {
  // LOG(INFO) << "k:" << k << " m:" << m << " n:" << n;
  // m and n can be zero
  CHECK(m >= 0 && n >= 0);
  CHECK(k > 0 && k <= (m + n));
  int i = static_cast<int>(static_cast<double>(m) / (m + n) * (k - 1));
  int j = k - 1 - i;
  CHECK(i >= 0 && i <= m) << "i:" << i << " j:" << j << " m:" << m;
  CHECK(j >= 0 && j <= n) << "i:" << i << " j:" << j << " n:" << n;

  type ai = ((i == m) ? MAX : a[i]);
  type bj = ((j == n) ? MAX : b[j]);
  type ai_1 = ((i == 0) ? MIN : a[i - 1]);
  type bj_1 = ((j == 0) ? MIN : b[j - 1]);

  if (ai >= bj_1 && ai <= bj) return ai;
  if (bj >= ai_1 && bj <= ai) return bj;
  if (ai < bj_1) {
    // delete a[0 ~ i] and b[j ~ n]
    return FindKthSmallest(a + i + 1, m - i - 1, b, j, k - i - 1, MAX, MIN);
  } else {
    // delete a[i ~ m] and b[0 ~ j]
    return FindKthSmallest(a, i, b + j + 1, n - j - 1, k - j - 1, MAX, MIN);
  }
}

template <typename type>
type FindKthSmallest(std::vector<type> & a, std::vector<type> & b,
                     int k, type MAX, type MIN) {
  CHECK(a.size() + b.size() > 0);
  int * foo = NULL;
  if (a.size() == 0)
    return FindKthSmallest(foo, 0, &(b[0]), b.size(), k, MAX, MIN);
  if (b.size() == 0)
    return FindKthSmallest(&a[0], a.size(), foo, 0, k, MAX, MIN);
  return FindKthSmallest(&a[0], a.size(), &b[0], b.size(), k, MAX, MIN);
}

}  // namespace algorithm

#endif  //  __KTH_H_
