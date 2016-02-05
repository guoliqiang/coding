// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 16:38:46
// File  : code.cc
// Brief :

/*
Given an array with n objects colored red, white or blue, sort them so that objects of the same
color are adjacent, with the colors in the order red, white and blue.
Here, we will use the integers 0, 1, and 2 to represent the color red, white, and blue respectively.

Note:
You are not suppose to use the library's sort function for this problem.
click to show follow up.

Follow up:
A rather straight forward solution is a two-pass algorithm using counting sort.
First, iterate the array counting number of 0's, 1's, and 2's, then overwrite array with total number of 0's, then 1's and followed by 2's.
Could you come up with an one-pass algorithm using only constant space?

*/

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
  for (int i = 0; i < n; i++) {  // 必须有&&后面的判断条件否则会死循环
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

namespace third {
void Sort(int A[], int n) {
  int b = -1;
  int e = n;
  int i = 0;
  while (i < e) { // 必须有&&后面的判断条件否则会死循环
    if (A[i] == 0 && i != b + 1) std::swap(A[i], A[++b]);
    else if (A[i] == 2 && i != e - 1) std::swap(A[i], A[--e]);
    else i++;
  }
}
}  // namespace third

namespace NB {

void SortColors(std::vector<int>& nums) {
  int idx0 = -1;
  int idx1 = -1;
  int idx2 = -1;
  for (int i = 0; i < nums.size(); i++) {
    if (nums[i] == 0) {
      nums[++idx2] = 2;
      nums[++idx1] = 1;
      nums[++idx0] = 0;
    } else if (nums[i] == 1) {
      nums[++idx2] = 2;
      nums[++idx1] = 1;
    } else {
      nums[++idx2] = 2;
    }
  }
}
}  // namespace NB


int main(int argc, char** argv) {
  int A[] = {2, 1, 1, 1, 0, 0, 2};
  int size = sizeof(A) / sizeof(int);
  Out(A, size);
  SortColors(A, size);
  Out(A, size);
  return 0;
}
