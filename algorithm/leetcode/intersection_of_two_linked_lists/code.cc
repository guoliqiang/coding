// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-15 17:48:57
// File  : code.cc
// Brief :
/*
Write a program to find the node at which the intersection of two singly linked lists begins.

For example, the following two linked lists:

A:          a1 → a2
                   ↘
                     c1 → c2 → c3
                   ↗
B:     b1 → b2 → b3
begin to intersect at node c1.


Notes:

If the two linked lists have no intersection at all, return null.
The linked lists must retain their original structure after the function returns.
You may assume there are no cycles anywhere in the entire linked structure.
Your code should preferably run in O(n) time and use only O(1) memory.
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

ListNode *GetIntersectionNode(ListNode *headA, ListNode *headB) {
  if (headA == NULL || headB == NULL) return NULL;

  int len_a = 1;
  int len_b = 1;
  ListNode * tail_a = headA;
  ListNode * tail_b = headB;
  while (tail_a->next != NULL) {
    len_a++;
    tail_a = tail_a->next;
  }
  while (tail_b->next != NULL) {
    len_b++;
    tail_b = tail_b->next;
  }
  if (tail_a != tail_b) return NULL;

  ListNode * longer = headA;
  ListNode * shorter = headB;
  int dis = fabs(len_a - len_b);
  if (len_b > len_a) {
    std::swap(longer, shorter);
  }

  for (int i = 0; i < dis; i++) {
    longer = longer->next;
  }
  while (longer != shorter) {
    longer = longer->next;
    shorter = shorter->next;
  }
  return longer;
}
}  // namespace algorithm

int main(int argc, char** argv) {
  return 0;
}
