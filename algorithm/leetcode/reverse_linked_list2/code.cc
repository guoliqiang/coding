// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 16:00:53
// File  : code.cc
// Brief :

/*
Reverse a linked list from position m to n. Do it in-place and in one-pass.
For example:
Given 1->2->3->4->5->NULL, m = 2 and n = 4,
return 1->4->3->2->5->NULL.

Note:
Given m, n satisfy the following condition:
1 ≤ m ≤ n ≤ length of list.

*/

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
struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x) : val(x), next(NULL) {}
};

namespace algorithm {

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


namespace third {
ListNode * Reverse(ListNode * head, int m, int n) {
  ListNode * rs = head;
  ListNode * pre = NULL;
  for (int i = 1; i < m; i++) {
    pre = head;
    head = head->next;
  }

  ListNode * l_head = NULL;
  ListNode * l_tail = NULL;
  for (int i = 0; i < n - m + 1; i++) {
    ListNode * next = head->next;
    head->next = l_tail;
    l_tail = head;
    if (l_head == NULL) l_head = l_tail;
    head = next;
  }
  if (pre != NULL) pre->next = l_tail;
  l_head->next = head;
  return pre == NULL ? l_tail : rs;
}

}  // namespace third

namespace twice {
class Solution {
 public:
  ListNode *reverseBetween(ListNode *head, int m, int n) {
    ListNode * rs = head;
    ListNode * pre = NULL;
    int i = m - 1;
    for (int j = 0; j < i; j++){
      pre = head;
      head = head->next;
    }
    i = n - m;
    ListNode * thead = NULL;
    ListNode * tail = NULL;
    for (int j = 0; j < i; j++) {
      ListNode * next = head->next;
      head->next = tail;
      tail = head;
      if (thead == NULL) thead = tail;
      head = next;
    }
    // head还需要翻转
    if (thead != NULL) thead->next = head->next;
    if (pre != NULL) pre->next = head;
    if (tail != NULL) head->next = tail;  // for case m == n

    if (pre == NULL) return head;
    return rs;
  }
};
}  // namespace twice

// 这种写法不容易出错
namespace four {
ListNode * Split(ListNode * head, int k) {
  ListNode * tmp = head;
  for (int i = 1; i < k ; i++) {
    tmp = tmp->next;
  }
  ListNode * rs = tmp->next;
  tmp->next = NULL;
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

ListNode * Reverse(ListNode * head, int m, int n) {
  if (m == n) return head;
  ListNode * last = Split(head, n);
  if (m == 1) {
    ListNode * t = Reverse(head);
    head->next = last;
    return t;
  } else {
    ListNode * b = Split(head, m - 1);
    ListNode * e = Reverse(b);
    b->next = last;
    ListNode * tmp = head;
    while (tmp->next != NULL) tmp = tmp->next;
    tmp->next = e;
    return head;
  }
}

}  // namespace four

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
