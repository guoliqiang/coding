// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 17:38:42
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class MedianFinder {
 public:
  // Adds a number into the data structure.
  void addNum(int num) {
    if (first.empty() || num < first.top()) {
      first.push(num);
    } else {
      second.push(num);
    }
    if (int(first.size()) - int(second.size()) > 1) {
      second.push(first.top());
      first.pop();
    }
    if (int(second.size()) - int(first.size()) > 1) {
      first.push(second.top());
      second.pop();
    }
  }

  // Returns the median of current data stream
  double findMedian() {
    if (first.size() == second.size()) {
      return (double(first.top()) + double(second.top())) / 2;
    } else {
      if (first.size() > second.size()) return first.top();
      else return second.top();
    }
  }

  std::priority_queue<int, std::vector<int>, std::less<int> > first;
  std::priority_queue<int, std::vector<int>, std::greater<int> > second;
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  MedianFinder foo;
  for (int i = 0; i < 10; i++) {
    LOG(INFO) << i;
    foo.addNum(i);
    LOG(INFO) << foo.findMedian();
  }
  return 0;
}
