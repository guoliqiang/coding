// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 15:36:42
// File  : code.cc
// Brief :

class Stack {
public:
    // Push element x onto stack.
    void push(int x) {
        queue.push_back(x);
    }

    // Removes the element on top of the stack.
    void pop() {
        queue.pop_back();
    }

    // Get the top element.
    int top() {
        return queue.back();
    }

    // Return whether the stack is empty.
    bool empty() {
        return queue.empty(); 
    }
    std::list<int> queue;
};
