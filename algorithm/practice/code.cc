// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-11-25 18:11:36
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
template <class T>
struct MinStack {
 public:
  void Push(T a) {
    stack.push(a);
    if (min.empty() || a < min.top().first) {
      min.push(std::make_pair(a, 1));
    } else if (a == min.top().first) {
      min.top().second++;
    }
  }
  
  T Min() {
    return min.top().first;
  }
  
  T Top() {
    return stack.top();
  }
  
  void Pop() {
    T tmp = stack.top();
    if (tmp == min.top().first) min.top().second--;
    if (min.top().second == 0) min.pop();
    stack.pop();
  }
  
 private:
   std::stack<T> stack;
   std::stack<std::pair<T, int> > min; 
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
 MinStack<int> stack;
 stack.Push(1);
 stack.Push(2);
 stack.Push(1);
 stack.Push(3);
 for (int i = 0; i < 4; i++) {
   LOG(INFO) << stack.Top() << " " << stack.Min();
   stack.Pop();
 }
 return 0;
}
