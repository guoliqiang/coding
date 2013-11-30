// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 20:13:52
// File  : code.cc
// Brief :

/*
Follow up for "Remove Duplicates":
What if duplicates are allowed at most twice?
For example,
Given sorted array A = [1,1,1,2,2,3],
Your function should return length = 5, and A is now [1,1,2,2,3].
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 22/22 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 88 milli secs
 * Progress: 164/164 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

void Assign(int * a, int from,
            int to, int * first, int * second) {
  if (to == -2) {
    *first = a[from];
    return;
  }
  if (to == -1) {
    *second = a[from];
    return;
  }
  a[to] = a[from];
}

int RemoveDuplicates(int * a, int n) {
  int v = -3;
  int first = 0;
  int second = 0;
  for (int i = 0; i < n; i++) {
    if (i - 2 < 0) {
      Assign(a, i, ++v, &first, &second);
    } else {
      if (a[i] == a[i - 1] && a[i - 2] == a[i]) continue;
      else Assign(a, i, ++v, &first, &second);
    }
  }
  v += 2;
  int rs = v + 1;
  while (v >= 0) {
    if (v == 1) {
      a[v] = second;
    } else if (v == 0){
      a[v] = first;
    } else {
      a[v] = a[v - 2];
    }
    v--;
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
int Remove(int A[], int n) {
  int first = 0;
  int second = 0;
  int k = -1;
  for (int i = 0; i < n; i++) {
      if (i == 0) first = A[i];
      else if (i == 1) second = A[i];
      else {
          if (A[i] != A[i - 1] || A[i] != A[i - 2]) {
              k++;
              A[k] = A[i];
          }
      }
  }
  k += 2;
  for (int i = k; i > 1; i--) {
      A[i] = A[i - 2];
  }
  A[1] = second;
  A[0] = first;
  return k + 1;
}
}  // namespace twice

namespace NB {
int Remove(int A[], int n) {
  int slow = 1;
  int fast = 2;
  int last = n > 0 ? A[0] : 0;
  while (fast < n) {
    if (A[fast] != last) {
      last = A[slow];
      A[++slow] = A[fast];
    } else {
      last = A[fast - 1];
    }
    fast++;
  }
  return n <= 2 ? n : slow + 1;
}
}  // namespace NB

int main(int argc, char** argv) {
  int A[] = {1, 1, 1, 2, 3};
  int size = sizeof(A) / sizeof(int);
  // int rs = RemoveDuplicates(A, size);
  // int rs = twice::Remove(A, size);
  int rs = NB::Remove(A, size);
  LOG(INFO) << rs;
  for (int i = 0; i < rs; i++) {
    LOG(INFO) << A[i];
  }
  return 0;
}
