// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-12 16:59:04
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class TwoSum {
public:
  // O(n)
  void add(int number) {
    std::list<int>::iterator it = data.begin();
    while (it != data.end() && *it < number) it++;
    data.insert(it, number);
  }
  // O(n)
  bool find(int value) {
    std::list<int>::iterator i = data.begin();
    std::list<int>::reverse_iterator j = data.rbegin();
    int size = data.size();
    while (size > 1) {
      if (*i + *j == value) return true;
      else if (*i + *j < value) {
        i++;
      } else {
        j++;
      }
      size--;
    }
    return false;
  }
  std::list<int> data;
};
}  // namespace algorithm

using namespace algorithm;

namespace NB {
class TwoSum {
public:
  // if use hash map add O(1), find O(n)
  void add(int number) {
    if (!data.count(number)) data[number] = 0;
    data[number]++;
  }
  bool find(int value) {
    for (std::map<int, int>::iterator i = data.begin(); i != data.end(); i++) {
      int v = value - i->first;
      if ((v == i->first && i->second > 2) ||
          (v != i->first && data.count(v))) {
        return true;
      }
    }
    return false;
  }
  std::map<int, int> data;
};
}  // namespace NB

int main(int argc, char** argv) {
  TwoSum foo;
  foo.add(1);
  foo.add(3);
  LOG(INFO) << foo.find(4);
  foo.add(5);
  LOG(INFO) << foo.find(4);
  LOG(INFO) << foo.find(5);
  foo.add(4);
  foo.add(1);
  LOG(INFO) << foo.find(5);
  return 0;
}
