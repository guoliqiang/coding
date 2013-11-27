
#include "base/public/common_head.h"

/*
Given a sorted array, remove the duplicates in place such that each element appear only once and return the new length.
Do not allocate extra space for another array, you must do this in place with constant memory.
For example,
Given input array A = [1,1,2],
Your function should return length = 2, and A is now [1,2].

*/

namespace algorithm {

void swap (int i, int j, int * p) {
    if (i == j) return;
    int foo = p[i];
    p[i] = p[j];
    p[j] = foo;
}

class Solution {
 public:
  int removeDuplicates(int A[], int n) {
    if (n == 0) return 0;
    int pre_value = A[0];
    int valid_index = 0;
    int len = 1;
    for (int i = 1; i < n; i++) {
      if (A[i] == pre_value) continue;
      else {
        pre_value = A[i];
        len ++;
        swap(valid_index + 1, i, A);
        valid_index++;
      }
    }
    return len;
  }
};
}  // namespace algorithm

namespace twice {
class Solution {
 public:
  int removeDuplicates(int A[], int n) {
    if (n == 0) return 0;
    int k = 0;
    for (int i = 1; i < n; i++) {
      if (A[i] != A[i - 1]) {
        A[++k] = A[i];
      }
    }
    return k + 1;
  }
};
}  // namespace twice


int main(int argc, char** argv) {
  return 0;
}
