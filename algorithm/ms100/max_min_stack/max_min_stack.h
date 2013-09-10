// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 15:40:47
// File  : max_min_stack.h
// Brief :

#ifndef  __MAX_MIN_STACK_H_
#define  __MAX_MIN_STACK_H_

#include <stack>
#include "base/public/logging.h"

namespace algorithm {

template <typename type>
class MaxMinStack {
 public:
  type top() {
    CHECK_GT(size(), 0);
    return stack_.top().value;
  }

  int size() {
    return stack_.size();
  }
  
  void pop() {
    stack_.pop();
  }

  type max() {
    CHECK_GT(size(), 0);
    return  stack_.top().max;
  }
  
  type min() {
    CHECK_GT(size(), 0);
    return  stack_.top().min;
  }

  void push(type i) {
    Node a;
    a.value = i;
    a.max = i;
    a.min = i;
    if (size() > 0) {
      a.max = i > max() ? i : max();
      a.min = i < min() ? i : min();
    }
    stack_.push(a);
  }

 private:
  struct Node {
    type value;
    type max;
    type min;
  };
 private:
  std::stack<Node> stack_;
};

}  // namespace algorithm

#endif  //  __MAX_MIN_STACK_H_
