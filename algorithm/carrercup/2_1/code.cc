// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 13:44:33
// File  : code.cc
// Brief :

/*
 * Write code to remove duplicates from an unsorted linked list.
 * FOLLOW UP
 * How would you solve this problem if a temporary buffer is not allowed?
 * 
 * */

#include "base/public/common_head.h"

namespace algorithm {

struct ListNode {
 int val;
 ListNode * next;
 ListNode(int v = 0) : val(v), next(NULL) {}
};

ListNode * RemoveDup(ListNode * head) {
  ListNode ** tail = &head;
  ListNode * t = head;
  while (t != NULL) {
    ListNode * foo = head;
    while (foo != *tail && foo->val != t->val) foo = foo->next;
    if (foo == *tail) {
      *tail = t;
      tail = &((*tail)->next);
    }
    t = t->next;
  }
  *tail = NULL;  // Note
  return head;
}

}  // namespace algorithm
using namespace algorithm;


int main(int argc, char** argv) {
  ListNode * head = new ListNode(1);
  head->next = new ListNode(1);
  head->next->next = new ListNode(2);
  ListNode * t = RemoveDup(head);
  while (t != NULL) {
    LOG(INFO) << t->val;
    t = t->next;
  }
  return 0;
}
