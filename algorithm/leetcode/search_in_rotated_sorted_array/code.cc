// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 12:59:22
// File  : code.cc
// Brief :

/*
Suppose a sorted array is rotated at some pivot unknown to you beforehand.

(i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).

You are given a target value to search. If found in the array return its index, otherwise return -1.

You may assume no duplicate exists in the array.
*/

#include "base/public/logging.h"

namespace NB {
int BSearch(int A[], int n, int t) {
  int b = 0;
  int e = n - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (A[mid] == t) return mid;
    else {
      if (A[mid] > A[e]) {  // 先判断mid在那部分
        if (t > A[e] && t < A[mid]) e = mid - 1;
        else b = mid + 1;
      } else {
        if (t <= A[e] && t > A[mid]) b = mid + 1;  // Note here is <= a[e]
        else e = mid - 1;
      }
    }
  }
  return -1;
}

// wrong
// TODO (guoliqiang) to fix it
int BSearch2(int A[], int n, int t) {
    int b = 0;
    int e = n - 1;
    while (b <= e) {
        int mid = b + (e - b) / 2;
        if (A[mid] == t) return mid;
        else {
           if (t > A[e]) {
             if (A[mid] > A[e] && t < A[mid]) e = mid - 1;
             else b = mid + 1;
           } else {
              // 分的类还不够详细
              // A[mid] > A[e] && t <= A[e] 此时应该b = mid + 1
              if (A[mid] <= A[e] && t > A[mid] && t <= A[e]) b = mid + 1;
              else e = mid - 1;
           }
        }
    }
    return -1;
}
}  // namespace NB

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 43/43 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 36 milli secs
 * Progress: 194/194 test cases passed.
 *
 * */

namespace algorithm {
bool BSearch(int * a, int b, int e, int n, int t, int * index) {
  // LOG(INFO) << "b: " << b << " e: " << e << " ";
  if (b > e) return false;
  if (t == a[n - 1]) {
    *index = n - 1;
    return true;
  }
  if (b == e && a[b] == t) {
    *index = b;
    return true;
  }

  int mid = b + (e - b) / 2;
  // LOG(INFO) << "mid:" << mid;
  if (a[mid] == t) {
    *index = mid;
    return true;
  } else {
    if (t > a[n - 1]) {  // t 处于左部
      if (a[mid] < t && a[mid] > a[n - 1])
        return BSearch(a, mid + 1, e, n, t, index);
      else
        return BSearch(a, b, mid - 1, n, t, index);
    } else {  // t处于右部
      if (a[mid] > t && a[mid] < a[n - 1])
        return BSearch(a, b, mid - 1, n, t, index);
      else
        return BSearch(a, mid + 1, e, n, t, index);
    }
  }
}

int BSearch(int * a, int n, int t) {
  int index = 0;
  if (BSearch(a, 0, n - 1, n, t, &index)) return index;
  return -1;
}

}  // namespace algorithm

namespace algorithm {
// recursive version
// call: 0, n - 1
bool BinarySearchR1(int * a, int b, int e, int t) {
  // 注意递归出口有两种，其中一种是非法的b,e,
  // 这种非法的b, e是迭代过程中产生的.
  if (b > e) return false;
  if (b == e && a[b] == t) return true;
  
  int mid = b + (e - b) / 2;
  if (a[mid] == t) return true;
  else {
    if (a[mid] > t) return BinarySearchR1(a, b, mid - 1, t);
    else return BinarySearchR1(a, mid + 1, e, t);
  }
}

// call: 0, n
bool BinarySearchR2(int * a, int b, int e, int t) {
  // 注意递归出口有两种，其中一种是非法的b,e,
  // 这种非法的b, e是迭代过程中产生的.
  if (b >= e) return false;
  if (b == e - 1 && a[b] == t) return true;

  int mid = b + (e - b) / 2;
  if (a[mid] == t) return true;
  else {
    if (a[mid] > t) return BinarySearchR2(a, b, mid, t);
    else return BinarySearchR2(a, mid + 1, e, t);
  }
}

// loop version
bool BinarySearchL1(int * a, int n, int t) {
  int b = 0;
  int e = n - 1;
  if (b > e) return false;

  while (b <= e) {  // bug fix
    int mid = b + (e - b) / 2;
    if (a[mid] == t) return true;
    else {
      if (a[mid] > t) e = mid - 1;
      else b = mid + 1;
    }
  }
  return false;
}

bool BinarySearchL2(int * a, int n, int t) {
  int b = 0;
  int e = n;
  if (b >= e) return false;

  while (b < e) {
    int mid = b + (e - b) / 2;
    if (a[mid] == t) return true;
    else {
      if (a[mid] > t) e = mid;
      else b = mid + 1;
    }
  }
  return false;
}

void BinarySearchTester(int * a, int n, int t) {
  LOG(INFO) << "find for:" << t;
  LOG(INFO) << "BinarySearchR1:" << BinarySearchR1(a, 0, n - 1, t);
  LOG(INFO) << "BinarySearchR2:" << BinarySearchR2(a, 0, n, t);
  LOG(INFO) << "BinarySearchL1:" << BinarySearchL1(a, n, t);
  LOG(INFO) << "BinarySearchL2:" << BinarySearchL2(a, n, t);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int A[] = {4,5,6,7,0,1,2};
  int n = sizeof(A)/ sizeof(int);

  LOG(INFO) << "5:" << BSearch(A, n, 5);
  LOG(INFO) << "4:" << BSearch(A, n, 4);
  LOG(INFO) << "2:" << BSearch(A, n, 2);
  LOG(INFO) << "7:" << BSearch(A, n, 7);
  LOG(INFO) << "0:" << BSearch(A, n, 0);
  
  int A1[] = {0,1,2,4,5,6,7};
  int n1 = sizeof(A1)/ sizeof(int);

  LOG(INFO) << "5:" << BSearch(A1, n1, 5);
  LOG(INFO) << "4:" << BSearch(A1, n1, 4);
  LOG(INFO) << "2:" << BSearch(A1, n1, 2);
  LOG(INFO) << "7:" << BSearch(A1, n1, 7);
  LOG(INFO) << "0:" << BSearch(A1, n1, 0);
  
  LOG(INFO) << "Test binary search";
  int bar[] = {};
  int size = 0;
  BinarySearchTester(bar, size, 10);
  
  std::vector<int> foo;
  foo.push_back(1);
  size = foo.size();
  BinarySearchTester(&foo[0], size, 1);
  BinarySearchTester(&foo[0], size, 10);
  
  foo.clear();
  foo.push_back(1);
  foo.push_back(2);
  size = foo.size();
  BinarySearchTester(&foo[0], size, 1);
  BinarySearchTester(&foo[0], size, 10);
  BinarySearchTester(&foo[0], size, 2);
  
  foo.clear();
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(3);
  size = foo.size();
  BinarySearchTester(&foo[0], size, 1);
  BinarySearchTester(&foo[0], size, 10);
  BinarySearchTester(&foo[0], size, 2);
  BinarySearchTester(&foo[0], size, 3);
  
  return 0;
}
