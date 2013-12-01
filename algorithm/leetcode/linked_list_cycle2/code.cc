// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-05 21:06:30
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

/*
Given a linked list, return the node where the cycle begins. If there is no cycle, return null.
Follow up:
Can you solve it without using extra space?

*/

/*
 * 16 / 16 test cases passed.
 * Status: Accepted
 * Runtime: 68 ms
 * Submitted: 0 minutes ago
 *
 * */
namespace algorithm {

struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x) : val(x), next(NULL) {}
};

ListNode * DetectCycle(ListNode * head) {
  ListNode * fast = head;
  ListNode * slow = head;
  do {
    if (fast == NULL || slow == NULL || fast->next == NULL) return NULL;
    slow = slow->next;
    fast = fast->next->next;
  } while (fast != slow);
  slow = head;
  while (slow != fast) {
    slow = slow->next;
    fast = fast->next;
  }
  return slow;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  return 0;
}
