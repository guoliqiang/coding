// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-21 02:12:26
// File  : code.cc
// Brief :


#include "base/public/common_ojhead.h"
#include "../ojlist/head.h"

namespace algorithm {

/*
 * 21 / 21 test cases passed.
 * Status: Accepted
 * Runtime: 236 ms
 * Submitted: 0 minutes ago
 * */

ListNode * InsertionSort(ListNode * head) {
  if (head == NULL) return NULL;
  
  ListNode * rs = head;
  ListNode * cur = head->next;
  rs->next = NULL;

  while (cur != NULL) {
    ListNode * next = cur->next;
    ListNode * pre = NULL;
    ListNode * temp = rs;
    while (temp != NULL) {
      if (temp->val > cur->val) break;
      pre = temp;
      temp = temp->next;
    }
    if (pre == NULL) {
      cur->next = rs;
      rs = cur;
    } else {
      pre->next = cur;
      cur->next = temp;
    }
    cur = next;
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
