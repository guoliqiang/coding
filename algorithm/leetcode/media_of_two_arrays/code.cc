// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-17 21:34:29
// File  : code.cc
// Brief :
#include "code.h"
#include "../k-th_smallest_of_two_sorted_array/k-th.h"
using namespace algorithm;

namespace twice {
int FindK(int * a, int m, int * b, int n, int k) {
  /* i + j = k -1
   * 其实a[0] ~ a[i] b[0] ~ b[j] 共有 k + 1个数
   * */
  int i = (k - 1) * (double) (m) / (double)(m + n);  // bug fixed
  int j = k - 1 - i;
  int ai = (i == m) ? INT_MAX : a[i];
  int ai_1 = (i == 0) ? INT_MIN : a[i - 1];
  int bj = (j == n) ? INT_MAX : b[j];
  int bj_1 = (j == 0) ? INT_MIN : b[j - 1];
  if (ai >= bj_1 && ai <= bj) return ai;
  if (bj >= ai_1 && bj <= ai) return bj;
  if (ai < bj_1) {
    return FindK(a + i + 1, m - i - 1, b, j, k - i - 1);  // 不可能是b[j], b[j] 被排除掉, 可反证
  } else {
    return FindK(a, i, b + j + 1, n - j - 1, k - j - 1);
  }
}
}

int main(int argc, char** argv) {
  int a[] = {100001};
  int b[] = {100000};
  int size = arraysize(a) + arraysize(b);
  double rs = 0;
  int t = size / 2;
  if (size % 2 == 1) {
    rs = twice::FindK(a, arraysize(a), b, arraysize(b), t + 1);
  } else {
    rs = (twice::FindK(a, arraysize(a), b, arraysize(b), t) +
          twice::FindK(a, arraysize(a), b, arraysize(b), t + 1)) / 2;
  }
  LOG(INFO) << rs;
  return 0;


  if (size % 2 == 0) {
    double foo = (FindKthSmallestWrapper(a, arraysize(a), b, arraysize(b), size/2 + 1) +
                  FindKthSmallestWrapper(a, arraysize(a), b, arraysize(b), size/2)) / 2.0;
    LOG(INFO) << FindKthSmallestWrapper(a, arraysize(a), b, arraysize(b), size/2);
    LOG(INFO) << foo;
  } else {
    LOG(INFO) << FindKthSmallestWrapper(a, arraysize(a), b, arraysize(b), size/2 + 1);
  }
  // LOG(INFO) << FindKthSmallestWrapper(a, 2, b, 4, 3);
  LOG(INFO) << findMedianSortedArrays(a, arraysize(a), b, arraysize(b));
  return 0;
}
