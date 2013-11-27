#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <map>

/*
 * Given an array of integers, find two numbers such that they add up to a specific target number.
 * The function twoSum should return indices of the two numbers such that they add up to the target,
 * where index1 must be less than index2. Please note that your returned answers (both index1 and index2) are not zero-based.
 * You may assume that each input would have exactly one solution.
 * Input: numbers={2, 7, 11, 15}, target=9
 * Output: index1=1, index2=2
 * */

#include "base/public/common_head.h"

namespace algorithm {

class Solution {
 public:
  std::vector<int> twoSum(std::vector<int> &numbers, int target) {
    std::map<int, int> foo;
    std::vector<int> rs;
    int index1 = 0;
    int index2 = 0;
    for (int i = 0; i< numbers.size(); i++) {
      foo.insert(std::make_pair(numbers[i], i));
    }
    for (int i = 0; i < numbers.size(); i++) {
      if (foo.count(target - numbers[i]) && i != foo[target - numbers[i]]) {
        index1 = i + 1;
        index2 = foo[target - numbers[i]] + 1;
        break;
      }
    }
    if (index1 > index2) std::swap(index1, index2);
    rs.push_back(index1);
    rs.push_back(index2);
    return rs;
  }
};

}  // namespace algorithm

/*
 * 10 / 10 test cases passed.
 * Status: Accepted
 * Runtime: 16 ms
 * Submitted: 0 minutes ago
 *
 * */


namespace NB {
class Solution {
 public:
  struct Node {
    int value;
    int index;
  };
  static bool Cmp(const Node & a, const Node & b) {
    return a.value < b.value;
  }
  std::vector<int> twoSum(std::vector<int> &numbers, int target) {
    std::vector<Node> foo;
    std::vector<int> rs;
    for (int i = 0; i < numbers.size(); i++) {
      Node n;
      n.value = numbers[i];
      n.index = i;
      foo.push_back(n);
    }
    sort(foo.begin(), foo.end(), Cmp);
    int index1 = 0;
    int index2 = foo.size() - 1;
    while (foo[index1].value + foo[index2].value != target) {
      if (foo[index1].value + foo[index2].value < target) index1++;
      if (foo[index1].value + foo[index2].value > target) index2--;
    }
    index1 = foo[index1].index + 1;
    index2 = foo[index2].index + 1;
    if (index1 > index2) std::swap(index1, index2);
    rs.push_back(index1);
    rs.push_back(index2);
    return rs;
  }
};
}  // namespace NB


int main(int argc, char** argv) {
  return 0;
}
