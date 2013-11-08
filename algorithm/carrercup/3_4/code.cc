// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 19:36:22
// File  : code.cc
// Brief :
// http://hawstein.com/posts/3.4.html

/* In the classic problem of the Towers of Hanoi, you have 3 rods
 * and N disks of different sizes which can slide onto any tower.
 * The puzzle starts with disks sorted in ascending order of size
 * from top to bottom (e.g., each disk sits on top of an even larger one).
 * You have the following constraints: 
 * (A) Only one disk can be moved at a time. 
 * (B) A disk is slid off the top of one rod onto the next rod. 
 * © A disk can only be placed on top of a larger disk. 
 * Write a program to move the disks from the first rod to the last using Stacks.
 * */


#include "base/public/common_head.h"

namespace algorithm {


void MoveDes(char cfrom, std::stack<int> & from,
             char cmid, std::stack<int> & mid,
             char cto, std::stack<int> & to, int n) {
  if (n == 1) {
    to.push(from.top());
    LOG(INFO) << "move " << from.top() << " from " << cfrom << " to " << cto;
    from.pop();
  } else {
    MoveDes(cfrom, from, cto, to, cmid, mid, n - 1);
    to.push(from.top());
    LOG(INFO) << "move " << from.top() << " from " << cfrom << " to " << cto;
    from.pop();
    MoveDes(cmid, mid, cfrom, from, cto, to, n - 1);
  }
}

void Move(std::stack<int> & from, std::stack<int> & mid, std::stack<int> & to, int n) {
  if (n == 1) {
    to.push(from.top());
    from.pop();
  } else {
    Move(from, to, mid, n - 1);
    to.push(from.top());
    from.pop();
    Move(mid, from, to, n - 1);
  }
}

struct Node {
  Node() {}
  Node(std::stack<int> * f, std::stack<int> * m, std::stack<int> * t, int tn)
      : from(f), mid(m), to(t), n(tn){}
 
  std::stack<int> * from;
  std::stack<int> * mid;
  std::stack<int> * to;
  int n;
};

void MoveIte(std::stack<int> & f, std::stack<int> & m, std::stack<int> & t, int ni) {
  std::stack<Node> stack;
  stack.push(Node(&f, &m, &t, ni));

  while (!stack.empty()) {
    Node t = stack.top();
    stack.pop();
    if (t.n == 1) {
      // LOG(INFO) << t.from->size() << " " << t.to->size() << " " << t.mid->size();
      t.to->push(t.from->top());
      t.from->pop();
    } else {
      // bug fix: push的顺序要与递归调用的顺序相反
      // 多次调用递归函数的入栈方式.
      stack.push(Node(t.mid, t.from, t.to, t.n - 1));
      stack.push(Node(t.from, t.mid, t.to, 1));
      stack.push(Node(t.from, t.to, t.mid, t.n - 1));
    }
  }
}


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::stack<int> from;
  std::stack<int> mid;
  std::stack<int> to;
  from.push(6);
  from.push(5);
  from.push(4);
  from.push(3);
  from.push(2);
  from.push(1);
  // Move(from, mid, to, from.size());
  MoveIte(from, mid, to, from.size());
  // MoveDes('f', from, 'm', mid, 't', to, from.size());

  while (!to.empty()) {
    LOG(INFO) << to.top();
    to.pop();
  }
  return 0;
}
