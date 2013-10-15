// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 16:38:46
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 36/36 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 28 milli secs
 * Progress: 86/86 test cases passed.
 *
 *
 * A rather straight forward solution is a two-pass algorithm using counting sort.
 * First, iterate the array counting number of 0's, 1's, and 2's,
 * then overwrite array with total number of 0's, then 1's and followed by 2's.
 *
 * 没想到用桶排序
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {
void Out(int * a, int n) {
  for (int i = 0; i < n; i++) {
    std::cout << a[i] << " ";
  }
  std::cout << std::endl;
}

void SortColors(int * a, int n) {
  int ptr_0 = -1;
  int ptr_2 = n;
  for (int i = 0; i < n ; i++) {
    if (a[i] == 0) std::swap(a[++ptr_0], a[i]);
    if (a[i] == 2 && i < ptr_2) {  // Note the condition i < ptr2
      std::swap(a[--ptr_2], a[i]);
      i--;
    }
  }
}



}  // namespace algorithm

using namespace algorithm;

namespace twice {
void Sort(int A[], int n) {
  int b = -1;
  int e = n;
  for (int i = 0; i < n; i++) {
      if (A[i] == 0 && i > b + 1) {
          std::swap(A[++b], A[i]);
          i--;
      } else if (A[i] == 2 && i < e - 1) {
          std::swap(A[--e], A[i]);
          i--;
      }
  }
}
}  // namespace twice


int main(int argc, char** argv) {
  int A[] = {2, 1, 1, 1, 0, 0, 2};
  int size = sizeof(A) / sizeof(int);
  Out(A, size);
  SortColors(A, size);
  Out(A, size);
  return 0;
}
