// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-05 20:58:17
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

/*
 * 16 / 16 test cases passed.
 * Status: Accepted
 * Runtime: 72 ms
 * Submitted: 0 minutes ago
 *
 * */

namespace algorithm {
struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x) : val(x), next(NULL){}
};

bool HasCycle(ListNode * head) {
  ListNode * fast = head;
  ListNode * slow = head;
  do {
    if (slow == NULL || fast == NULL || fast->next == NULL) return false;
    slow = slow->next;
    fast = fast->next->next;
  }  while (fast != slow);
  return true;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
