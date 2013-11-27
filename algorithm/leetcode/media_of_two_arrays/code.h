// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-17 20:12:34
// File  : code.h

/*
 * There are two sorted arrays A and B of size m and n respectively. Find the median of the two sorted arrays.
 * The overall run time complexity should be O(log (m+n)).
 * */

// Brief :
//     m0 m1 m2 m3       len = 4   len/2 = 2
//           |
//  n0 n1 n2 n3 n4 n5    len = 6   len/2 = 3
//  
//  if m2<= n3 
//  {n4, n5} >= {n0, n1, n2, n3, m0, m1, m2}  so delete {n4 n5} is ok.
//  {m0, m1} <= {m2 m3 n3 n4 n5} 也就是两个数组从小到大排好序后，m1右侧至少有5个数字
//  但肯有可能是这样的 n0 n1 n2 m0 m1 m2 m3 n3 n4 n5,所以m0可以删去但m1不可以
//
//  若 m数组各个为奇数 e.g. 5
//  {m0 m1} <= {m2 m3 m4 n3 n4 n5} 排好序后m1右侧至少6个数字，m1可以删除
//
//  
//  if m2 > n3
//  {m3} > {m0, m1, m2, n0, n1, n2, n3} so delete {m3} is ok.
//  {n0, n1, n2} < {m2, m3, n3, n4, n5} 同样两个数组排好序后，很有可能是这样的
//  m0 m1 n0 n1 n2 m2 m3 n3 n4 n5，所以n2不可以删去
//
//  若n数组的个数为奇数e.g. 7
//  {n0 n1 n2} <= {m2 m3 n3 n4 n5 n6} 同理n2 可以删去
//
//  k = min(i-1, n-j-1) when Ai <= Bj and m is even.
//  k = min(i, n-j-1)   when Ai <= Bj and m is odd.
//  k = min(m-i-1, j-1) when Ai > Bj  and n is even.
//  k = min(m-i-1, j)   when Ai > Bj  and n is odd.

#ifndef  __CODE_H_
#define  __CODE_H_

#include "base/public/logging.h"
#include "base/public/string_util.h"
#include <algorithm>
#include <vector>
#include <stdlib.h>

namespace algorithm {
// Note: med1 < = med2
// n != 1
double findMedianBaseCase2(int med1, int med2, int * B, int n) {
  std::vector<int> foo; 
  foo.push_back(med1);
  foo.push_back(med2);
  if (n % 2 == 0) {
    int a = (n/2 - 2) >= 0 ? B[n/2 - 2] : kint32min;
    int b = B[n/2 - 1];
    int c = B[n/2];
    int d = (n/2 + 1) < n ? B[n/2 + 1] : kint32max;
    // a<b<c<d
    foo.push_back(a);
    foo.push_back(b);
    foo.push_back(c);
    foo.push_back(d);
    sort(foo.begin(), foo.end());
    // LOG(INFO) << JoinVector(foo);
    return (foo[foo.size() / 2] + foo[foo.size() / 2 - 1]) / 2.0;
  } else {
    int a = B[n/2 - 1];
    int b = B[n/2];
    int c = B[n/2 + 1];
    foo.push_back(a);
    foo.push_back(b);
    foo.push_back(c);
    sort(foo.begin(), foo.end());
    return foo[foo.size() / 2];
  }
}

double findMedianBaseCase(int med, int * B, int n) {
  if (n == 1) return (med + B[0]) / 2.0;
  if (n % 2 == 0) {
    int a = B[n/2 - 1];
    int b = B[n/2];
    if (med <= a) return a;
    else if (med <= b) return med;
    else return b;
  } else {
    int a = B[n/2 - 1];
    int b = B[n/2];
    int c = B[n/2 + 1];
    if (med <= a) return (a + b) / 2.0;
    else if (med <= c) return (med + b) / 2.0;
    else return (b + c) / 2.0;
  }
}

double findMedianSingleArray(int * B, int n) {
  return (n % 2 == 1) ? B[n/2] : (B[n/2 - 1] + B[n/2]) / 2.0;
}

double findMedianSortedArrays(int A[], int m, int B[], int n) {
  CHECK(m + n >= 1);
   
  if (m == 0) return findMedianSingleArray(B, n);
  if (n == 0) return findMedianSingleArray(A, m);
  // need to deal with case = 1 and 2
  if (m == 1) return findMedianBaseCase(A[0], B, n);
  if (n == 1) return findMedianBaseCase(B[0], A, m);
  if (m == 2) return findMedianBaseCase2(A[0], A[1], B, n);
  if (n == 2) return findMedianBaseCase2(B[0], B[1], A, m);
 
  int i = m/2;
  int j = n/2;
  int k;
  if (A[i] <= B[j]) {
    k = ((m%2 == 0) ? std::min(i-1, n-j-1) : std::min(i, n-j-1));
    CHECK(k > 0);
    return findMedianSortedArrays(A+k, m-k, B, n-k);
  } else {
    k = ((n%2 == 0) ? std::min(m-i-1, j-1) : std::min(m-i-1, j));
    CHECK(k > 0);
    return findMedianSortedArrays(A, m-k, B+k, n-k);
  }
}


}  // namespace algorithm

#endif  // __CODE_H_
