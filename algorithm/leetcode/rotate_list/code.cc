// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 16:44:32
// File  : code.cc
// Brief :
//
/*
Given a list, rotate the list to the right by k places, where k is non-negative.

For example:
Given 1->2->3->4->5->NULL and k = 2,
return 4->5->1->2->3->NULL.

*/
// if (-1)  == if (true)
/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 29/29 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 60 milli secs
 * Progress: 229/229 test cases passed
 *
 * */

#include <vector>
#include "base/public/common_head.h"

struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x) : val(x), next(NULL) {}
};


namespace algorithm {
int Length(ListNode * head) {
  int rs = 0;
  while (head) {
    rs++;
    head = head->next;
  }
  return rs;
}

ListNode * RotateRight(ListNode * head, int k) {
  int len = Length(head);
  if (len == 0) return head;
  k %= len;  // Note len == 0 出core
  if (k == 0) return head;
  ListNode * p1 = head;
  ListNode * p2 = head;
  while (k) {
    p1 = p1->next;
    k--;
  }

  while (p1->next != NULL) {
    p1 = p1->next;
    p2 = p2->next;
  }
  p1->next = head;
  head = p2->next;
  p2->next = NULL;
  return head;
}

void Out(ListNode * head) {
  while (head) {
    LOG(INFO) << head->val;
    head = head->next;
  }
}

ListNode * Build(std::vector<int> & v) {
  ListNode * head = NULL;
  ListNode * pre = NULL;
  for (int i = 0; i < v.size(); i++) {
    if (head == NULL) {
      head = new ListNode(v[i]);
      pre = head;
    } else {
      pre->next = new ListNode(v[i]);
      pre = pre->next;
    }
  }
  return head;
}

}  // namespace algorithm

namespace twice {
ListNode *RotateRight(ListNode *head, int k) {
  ListNode * ptr1 = head;
  int n = 0;
  while (ptr1 != NULL) {
    n++;
    ptr1 = ptr1->next;
  }
  if (n == 0) return head;
  k %= n;
  // 保证 0 < k < n
  ptr1 = head;
  ListNode * ptr2 = head;
  while (k) {
    k--;
    ptr1 = ptr1->next;
  }
  while (ptr1->next != NULL) {
    ptr1 = ptr1->next;
    ptr2 = ptr2->next;
  }
  ptr1->next = head;
  head = ptr2->next;
  ptr2->next = NULL;
  return head;
}
}  // namespace twice

namespace third {
ListNode* RotateRight(ListNode* head, int k) {
        ListNode * cur = head;
        int n = 0;
        while (cur != NULL) {
            cur = cur->next;
            n++;
        }
        
        if (n == 0 || k % n == 0) return head; 
        k = k % n;
        
        cur = NULL;
        ListNode * pre = NULL;
        for (int i = 0; i <= n - k; i++) {
            pre = cur;
            if (i == 0) cur = head;
            else cur = cur->next;
        }
        
        ListNode * ans = cur;
        while (cur->next != NULL) cur = cur->next;
        
        cur->next = head;
        if (pre != NULL) pre->next = NULL;
        return ans;
    }
}  // namespace third

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 1; i < 3; i++) {
    foo.push_back(i);
  }
  ListNode * head = Build(foo);
  Out(head);
  HERE(INFO);
  head = third::RotateRight(head, 1);
  Out(head);
  return 0;
}
