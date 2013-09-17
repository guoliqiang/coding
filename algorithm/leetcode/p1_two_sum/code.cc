#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <map>

// 20 ms
class Solution {
public:
    vector<int> twoSum(vector<int> &numbers, int target) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
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

// also 20 ms. confused...
class Solution {
public:
    struct Node {
      int value;
      int index;
    };
    static bool Cmp(const Node & a, const Node & b) {
      return a.value < b.value;
    }
    vector<int> twoSum(vector<int> &numbers, int target) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        std::vector<Node> foo;
        std::vector<Node> rs;
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
