// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-21 02:12:26
// File  : code.cc
// Brief :

/*
Sort a linked list using insertion sort.
*/

#include "base/public/common_ojhead.h"
#include "../ojlist/head.h"

namespace algorithm {

/*
 * 21 / 21 test cases passed.
 * Status: Accepted
 * Runtime: 236 ms
 * Submitted: 0 minutes ago
 * */

// 前面牌号序的末尾指针一定要独立出来，这样不容易出错
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


// 由于前面排好序的那段没有独立出来，case : 2 ,1 出现了死循环 
ListNode * InsertSort(ListNode * head) {
  if (head == NULL) return head;
  ListNode * tmp = head->next;
  while (tmp != NULL) {
    ListNode * next = tmp->next;
    ListNode * pre = NULL;
    ListNode * cur = head;
    while (cur != tmp) {
      if (cur->val > tmp->val) break;
      pre = cur;
      cur = cur->next;
    }
    if (cur != tmp) {
      if (pre == NULL) {
        tmp->next = head;
        head = tmp;
      } else {
        tmp->next = pre->next;
        pre->next = tmp;
      }
    }
    tmp = next;
  }
  return head;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
