// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 15:36:42
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

class Stack {
public:
    // Push element x onto stack.
    void push(int x) {
        if (q1.size()) q1.push(x);
        else if (q2.size()) q2.push(x);
        else q1.push(x);
    }

    // Removes the element on top of the stack.
    void pop() {
        if (q1.size()) {
            int size = q1.size();
            for (int i = 0; i < size - 1; i++) {
                q2.push(q1.front());
                q1.pop();
            }
            q1.pop();
        } else {
            int size = q2.size();
            for (int i = 0; i < size - 1; i++) {
                q1.push(q2.front());
                q2.pop();
            }
            q2.pop();
        }
    }

    // Get the top element.
    int top() {
        int ans = 0;
        if (q1.size()) {
            while (q1.size()) {
                ans = q1.front();
                q2.push(q1.front());
                q1.pop();
            }
        } else {
            while (q2.size()) {
                ans = q2.front();
                q1.push(q2.front());
                q2.pop();
            }
        }
        return ans;
    }

    // Return whether the stack is empty.
    bool empty() {
        return q1.empty() && q2.empty();
    }
    std::queue<int> q1;
    std::queue<int> q2;
};


namespace NB {
class Stack {
public:
    // Push element x onto stack.
    void push(int x) {
      queue.push(x);
      int size = queue.size() - 1;
      for (int i = 0; i < size; i++) {
        int t = queue.front();
        queue.pop();
        queue.push(t);
      }
    }

    // Removes the element on top of the stack.
    void pop() {
      queue.pop();
    }

    // Get the top element.
    int top() {
      return queue.front();
    }

    // Return whether the stack is empty.
    bool empty() {
        return queue.empty();
    }
    std::queue<int> queue;
};
}  // namespace NB

int main(int argc, char** argv) {
  Stack s;
  NB::Stack s2;
  s.push(1);
  s2.push(1);
  s.push(2);
  s2.push(2);
  s.push(3);
  s2.push(3);
  LOG(INFO) << s.top();
  LOG(INFO) << s2.top();
  s.pop();
  s2.pop();
  LOG(INFO) << s.top();
  LOG(INFO) << s2.top();
  s.pop();
  s2.pop();
  LOG(INFO) << s.top();
  LOG(INFO) << s2.top();
  s.pop();
  s2.pop();
  return 0;
}
