// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 16:44:32
// File  : code.cc
// Brief :
//
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

namespace algorithm {
struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x) : val(x), next(NULL) {}
};


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

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 0; i < 10; i++) {
    foo.push_back(i);
  }
  ListNode * head = Build(foo);
  Out(head);
  HERE(INFO);
  head = RotateRight(head, 0);
  Out(head);
  return 0;
}
