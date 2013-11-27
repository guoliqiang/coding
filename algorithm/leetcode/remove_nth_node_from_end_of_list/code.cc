// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-11-26 10:27:09
// File  : code.cc
// Brief :

/*
Given a linked list, remove the nth node from the end of list and return its head.
For example,
Given linked list: 1->2->3->4->5, and n = 2.
After removing the second node from the end, the linked list becomes 1->2->3->5.

Note:
Given n will always be valid.
Try to do this in one pass.
*/

#include "base/public/common_head.h"

/*
Run Status: Accepted!
Program Runtime: 8 milli secs
Progress: 7/7 test cases passed.

Run Status: Accepted!
Program Runtime: 40 milli secs
Progress: 207/207 test cases passed.
*/

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
   ListNode *removeNthFromEnd(ListNode *head, int n) {
      ListNode * ptr1 = head;
      ListNode * ptr2 = head;
      while (n) {
        ptr1 = ptr1->next;
        n--;
      }
      // 因为输入的ｎ保证是有效的
      if (ptr1 == NULL) return head->next;  // Note bug fixed
      while (ptr1->next != NULL) {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
      }
      ptr2->next = ptr2->next->next;
      return head;
    }
};


int main(int argc, char** argv) {
  return 0;
}
