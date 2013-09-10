// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 19:07:06
// File  : stack2queue.h
// Brief :

#ifndef  __STACK2QUEUE_H_
#define  __STACK2QUEUE_H_

#include <stack>
#include "base/public/logging.h"

namespace algorithm {

template <typename type>
class MyQueue {
 public:
  void Push(type & i) {
    a_.push(i);
  }

  void Pop() {
    Move();
    b_.pop();
  }

  type & Front() {
    Move();
    return b_.top();
  }

  bool Empty() {
    return a_.empty() && b_.empty();
  }

 private:
  void Move() {
    if (b_.empty()) {
      while(!a_.empty()) {
        b_.push(a_.top());
        a_.pop();
      }
    }
  }

 private:
  std::stack<type> a_;
  std::stack<type> b_;
};

}  // namepsace algorithm

#endif  //  __STACK2QUEUE_H_
