// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-05 21:13:28
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

/*
 *
 * 13 / 13 test cases passed.
 * Status: Accepted
 * Runtime: 324 ms
 * Submitted: 0 minutes ago
 *
 * */

namespace algorithm {

struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x) : val(x), next(NULL){}
};

void ReorderList(ListNode * head) {
  int len = 0;
  ListNode * foo = head;
  while (foo != NULL) {
    len++;
    foo = foo->next;
  }
  if (len == 0) return;
  int l = len;
  len /= 2;
  foo = head;
  while (len) {
    foo = foo->next;
    len--;
  }
  ListNode * pre = NULL;
  while (foo != NULL) {
    ListNode * t = foo->next;
    foo->next = pre;
    pre = foo;
    foo = t;
  }
  foo = head;
  for (int i = 0; i < l / 2; i++) {
    ListNode *t1 = foo->next;
    ListNode *t2 = pre->next;
    foo->next = pre;
    pre->next = t1;
    foo = t1;
    pre = t2;
  }
  foo->next = NULL;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  ListNode * head = new ListNode(1);
  head->next = new ListNode(2);
  ReorderList(head);
  return 0;
}
