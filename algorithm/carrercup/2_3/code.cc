// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 14:33:59
// File  : code.cc
// Brief :

/*
 * Implement an algorithm to delete a node in the middle of a single linked list, 
 * given only access to that node.
 *
 * EXAMPLE
 *
 * Input: the node ‘c’ from the linked list a->b->c->d->e Result: nothing is returned,
 * but the new linked list looks like a->b->d->e
 *
 * 当要删除的元素为最后一个元素时，只能通过设定特殊标志位来处理，因为没有提供head指针。
 * */

#include "base/public/common_head.h"

namespace algorithm {

struct ListNode {
  int val;
  ListNode * next;
  ListNode (int v = 0) : val(v), next(NULL) {}
};

bool RemoveMiddleNode(ListNode * remove) {
  if (remove->next != NULL) {
    remove->val = remove->next->val;
    remove->next = remove->next->next;
    return true;
  }
  return false;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
