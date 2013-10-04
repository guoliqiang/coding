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

/*
 * 避免了保存多份min 和　max
 * http://hawstein.com/posts/3.2.html
 * */
namespace NB {

template <typename type>
class MaxMinStack {
 public:
  struct Node {
    Node(type i) : v(i), count(1){};
    Node() : count(-1) {}
    type v;
    int count;
  };

  type Max() {
    return max_.top().v;
  }

  type Min() {
    return min_.top().v;
  }

  // bug fixed, 1, last push
  //            2, use t to store temporary value
  void Push(type i) {
    if (Size() <= 0) {
      min_.push(Node(i));
    } else {
      int t = Min();
      if (i < t) {
        min_.push(Node(i));
      } else if (i == t) {
        min_.top().count++;
      }
    }

    if (Size() <= 0) {
      max_.push(Node(i));
    } else {
      int t = Max();
      if (i > t) {
        max_.push(Node(i));
      } else if (i == t) {
        max_.top().count++;
      }
    }
    stack_.push(i);
  }

  type Top() {
    return stack_.top();
  }

  void Pop() {
    if (Max() == stack_.top()) {
      max_.top().count--;
      if (max_.top().count == 0) max_.pop();
    }

    if (Min() == stack_.top()) {
      min_.top().count--;
      if (min_.top().count == 0) min_.pop();
    }
    stack_.pop();
  }

  int Size() {
    return stack_.size();
  }

 private:
  std::stack<type> stack_;
  std::stack<Node> min_;
  std::stack<Node> max_;
};
}  // namespace NB

#endif  //  __MAX_MIN_STACK_H_
