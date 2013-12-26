// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-05 21:13:28
// File  : code.cc
// Brief :

/*
Given a singly linked list L: L0→L1→…→Ln-1→Ln,
reorder it to: L0→Ln→L1→Ln-1→L2→Ln-2→…

You must do this in-place without altering the nodes' values.

For example,
Given {1,2,3,4}, reorder it to {1,4,2,3}.

*/

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

// 这个思路更清晰，split和reverse的写法都不错
namespace twice {
ListNode * Split(ListNode * head, int len) {
  for (int i = 1; i < len; i++) {
    head = head->next;
  }
  ListNode * rs = head->next;
  head->next = NULL;
  return rs;
}

ListNode * Reverse(ListNode * head) {
  ListNode * tail = NULL;
  while (head != NULL) {
    ListNode * next = head->next;
    head->next = tail;
    tail = head;
    head = next;
  }
  return tail;
}

void ReOrder(ListNode * head) {
  int count = 0;
  ListNode * tmp = head;
  while (tmp != NULL) {
    count++;
    tmp = tmp->next;
  }
  if (count <= 1) return;
  int len = (count + 1) / 2;
  ListNode * l1 = head;
  ListNode * l2 = Split(head, len);
  l2 = Reverse(l2);
  ListNode * rs = NULL;
  ListNode ** pre = &rs;
  while (l1 != NULL || l2 != NULL) {
    if (l1 != NULL) {
      *pre = l1;
      pre = &((*pre)->next);
      l1 = l1->next;
    }
    if (l2 != NULL) {
      *pre = l2;
      pre = &((*pre)->next);
      l2 = l2->next;
    }
  }
}
}  // namespace twice

int main(int argc, char** argv) {
  ListNode * head = new ListNode(1);
  head->next = new ListNode(2);
  ReorderList(head);
  return 0;
}
