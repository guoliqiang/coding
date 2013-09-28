// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 13:51:41
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 13/13 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 24 milli secs
 * Progress: 59/59 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

void MergeNB(int * a, int m, int *b, int n) {
  while(n > 0){
    if(m <= 0 || A[m - 1] < B[n - 1]) A[n + m - 1] = B[--n]; // []优先级高于++/--  从左到右
    else A[n + m - 1] = A[--m];
  }
}

void Merge(int * a, int m, int * b, int n) {
  int m_ptr = m + n - 1;
  int a_ptr = m - 1;
  int b_ptr = n - 1;
  
  while (a_ptr >= 0 && b_ptr >= 0)
    a[m_ptr--] = (a[a_ptr] > b[b_ptr]) ? a[a_ptr--] : b[b_ptr--];
  
  while (b_ptr >= 0) a[m_ptr--] = b[b_ptr--];
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int A[] = {0, 1, 2, 3, 0, 0, 0};
  int B[] = {1};
  Merge(A, 0, B, 1);
  for (int i = 0; i < 1; i ++) {
    LOG(INFO) << A[i];
  }
  return 0;
}
