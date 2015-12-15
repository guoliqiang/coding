// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-15 16:05:26
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

class MinStack {
 public:
  void push(int x) {
    int min = x;
    if (size() > 0) min = std::min(min, getMin());
    data_.push(std::make_pair(x, min));
  }

  void pop() { data_.pop(); }
  int top() { return data_.top().first; }
  int getMin() { return data_.top().second; }
  int size() { return data_.size(); }

 private:
  std::stack<std::pair<int, int> > data_;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  MinStack min_stack;
  for (int i = 10; i > 0; i--) {
    min_stack.push(i);
  }

  for (int i = 10; i > 0; i--) {
    LOG(INFO) << min_stack.top() << " " << min_stack.getMin();
    min_stack.pop();
  }
  return 0;
}
