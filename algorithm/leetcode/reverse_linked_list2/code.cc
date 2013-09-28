// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 16:00:53
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 24/24 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 20 milli secs
 * Progress: 44/44 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x) : val(x), next(NULL) {}
};

void Out(ListNode * root) {
  while (root) {
    std::cout << root->val << " ";
    root = root->next;
  }
  std::cout << std::endl;
}

ListNode * MakeList(std::vector<int> & v) {
  ListNode * head = NULL;
  ListNode * tail = NULL;
  for (int i = 0; i < v.size(); i++) {
    if (head == NULL) {
      head = new ListNode(v[i]);
      tail = head;
    } else {
      tail->next = new ListNode(v[i]);
      tail = tail->next;
    }
  }
  return head;
}

void ReverseS(ListNode * h) {
  ListNode * pre = NULL;
  while (h != NULL) {
    ListNode * foo = h->next;
    h->next = pre;
    pre = h;
    h = foo;
  }
}

void Reverse(ListNode * h) {
  if (h->next == NULL) return;

  ListNode * pre = NULL;
  while (h != NULL && h->next != NULL) {
    // 偶数时h == NULL
    ListNode * foo = h->next->next;
    ListNode * bar = h->next;
    h->next->next = h;
    h->next = pre;
    pre = bar;
    h = foo;
  }
  // 注意处理奇数末尾结点
  if (h != NULL) h->next = pre;
}

ListNode * Reverse(ListNode * head, int m, int n) {
  ListNode * pre_m = head;
  ListNode * p_n = head;
  while (m - 1 > 1) {
    pre_m = pre_m->next;
    m--;
  }
  if (m == 1) pre_m = NULL;

  while (n > 1) {
    p_n = p_n->next;
    n--;
  }

  ListNode * foo = p_n->next;
  p_n->next = NULL;
  
  if (pre_m != NULL) {
    ReverseS(pre_m->next);
    pre_m->next->next = foo;
    pre_m->next = p_n;
    return head;
  } else {
    ReverseS(head);
    head->next = foo;
    return p_n;
  }
}

}  // nemespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(3);
  foo.push_back(4);
  ListNode * head = MakeList(foo);
  Out(head);
  Out(Reverse(head, 2, 3));
  return 0;
}
