// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-11 15:42:16
// File  : code.cc
// Brief :

/*
A linked list is given such that each node contains an additional random pointer which could point to any node in the list or null.
Return a deep copy of the list.

*/

/*
 *
 * 11 / 11 test cases passed.
 * Status: Accepted
 * Runtime: 296 ms
 * Submitted: 0 minutes ago
 *
 * */
#include "base/public/common_head.h"

namespace algorithm {

struct RandomListNode {
  int label;
  RandomListNode * next;
  RandomListNode * random;
  RandomListNode(int x) : label(x), next(NULL), random(NULL){}
};

RandomListNode * CopyRandomList(RandomListNode * node) {
  RandomListNode * t = node;
  while (t != NULL) {
    RandomListNode * foo = new RandomListNode(t->label);
    RandomListNode * bar = t->next;
    t->next = foo;
    foo->next = bar;
    t = bar;
  }
  t = node;
  while (t != NULL) {
    if (t->random != NULL) {
      t->next->random = t->random->next;
    }
    t = t->next->next;
  }
  RandomListNode * head = NULL;
  RandomListNode ** tail = &head;
  t = node;
  while (t != NULL) {
    *tail = t->next;
    tail = &((*tail)->next);
    t->next = t->next->next;
    t = t->next;
  }
  // *tail = NULL;  // 可以省略
  return head;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
