// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 18:03:58
// File  : code.cc
// Brief :

/*
 * Imagine a (literal) stack of plates. If the stack gets too high, it might topple. 
 * Therefore, in real life, we would likely start a new stack when the previous stack
 * exceeds some threshold. Implement a data structure SetOfStacks that mimics this.
 * SetOfStacks should be composed of several stacks, and should create a new stack once
 * the previous one exceeds capacity. SetOfStacks.push() and SetOfStacks.pop() should behave
 * identically to a single stack (that is, pop() should return the same values as
 * it would if there were just a single stack).
 *
 * FOLLOW UP
 * Implement a function popAt(int index) which performs a pop operation on a specific sub-stack.
 * */

#include "base/public/common_head.h"

namespace algorithm {

// 如果count!= 0 保证最后一个栈非空
template <typename type>
class SetOfStacks {
 public:
  SetOfStacks(int per_count) : per_count_(per_count), count_(0) {}
  
  void Push(type i) {
    if (count_ == 0 || v_.back()->size() == per_count_) {
      v_.push_back(new std::stack<type>());
    }
    v_.back()->push(i);
    count_++;
  }

  type Top() {
    return count_ == 0 ? 0 : v_.back()->top();
  }

  int Size() {
    return count_;
  }

  void Pop() {
    if (count_ == 0) return;
    v_.back()->pop();
    while (v_.size() != 0 && v_.back()->size() == 0) {
      delete v_.back();
      v_.pop_back();
    }
    count_--;
  }

  void PopAt(int i) {
    if (count_ == 0 || i < 0) return;
    // bug fixed:
    // if i < 0 it will be larger than v_.size()
    // because v_.size() 's type is unsigned it will convert
    // i to unsigned so, -1 > v.size()
    if (i >= v_.size() - 1) { 
      Pop();
    } else {
      while (i >= 0 && v_[i]->size() == 0) i--;
      if (i >= 0) {
        LOG(INFO) << "pop " << i;
        v_[i]->pop();
        count_--;
      }
    }
  }

 private:
  int per_count_;
  int count_;
  std::vector<std::stack<type> *> v_;
};

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  SetOfStacks<int> s(5);
  for (int i = 0; i < 20; i++) s.Push(i);

  s.PopAt(3);
  s.PopAt(-100);
  s.PopAt(-2);

  while (s.Size() > 0) {
    LOG(INFO) << s.Top();
    s.Pop();
  }
  return 0;
}
