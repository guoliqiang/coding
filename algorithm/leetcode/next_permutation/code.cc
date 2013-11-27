
/*
Implement next permutation, which rearranges numbers into the lexicographically next greater permutation of numbers.
If such arrangement is not possible, it must rearrange it as the lowest possible order (ie, sorted in ascending order).
The replacement must be in-place, do not allocate extra memory.

Here are some examples. Inputs are in the left-hand column and its corresponding outputs are in the right-hand column.
1,2,3 → 1,3,2
3,2,1 → 1,2,3
1,1,5 → 1,5,1

*/

#include "base/public/common_head.h"

namespace algorithm {

class Solution {
 public:
  void nextPermutation(std::vector<int> &num) {
    int pre = num[num.size() - 1];
    int i = 0;
    for (i = num.size() - 2; i >= 0; i--) {
      if (num[i] < pre) break;
      else {
        pre = num[i];
      }
    }
    int j = 0;
    for (j = num.size() - 1; j > i; j--) {
      if (num[j] > num[i]) break;
    }
    std::swap(num[i], num[j]);
    // 注意翻转 i+1 到 字符串末尾 这所有之间的数字
    i++;
    j = num.size() - 1;
    while (i < j) {
      std::swap(num[i], num[j]);
      i++;
      j--;
    }
  }
};

}  // namespace algorithm


int main(int argc, char** argv) {
  return 0;
}
