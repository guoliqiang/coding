// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 19:10:26
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class Queue {
public:
    // Push element x to the back of queue.
    void push(int x) {
        front.push(x);
    }

    // Removes the element from in front of queue.
    void pop(void) {
        if (end.empty()) transfer();
        end.pop();
    }

    // Get the front element.
    int peek(void) {
        if (end.empty()) transfer();
        return end.top();
    }

    // Return whether the queue is empty.
    bool empty(void) {
        if (end.empty()) transfer();
        return end.empty();
    }

    void transfer() {
        while (front.size() > 0) {
            end.push(front.top());
            front.pop();
        }
    }
    std::stack<int> front;
    std::stack<int> end;
};
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  Queue q;
  q.push(1);
  LOG(INFO) << q.peek();
  return 0;
}
