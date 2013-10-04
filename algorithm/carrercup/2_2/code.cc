// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 14:15:23
// File  : code.cc
// Brief :

/*
 * 另外一种思路基于栈，实现时使用递归
 * http://hawstein.com/posts/2.2.html
 * */

#include "base/public/common_head.h"

namespace algorithm {

struct ListNode {
  int val;
  ListNode * next;
  ListNode(int v = 0) : val(0), next(NULL) {}
};

int LastNth(ListNode * head, int n, ListNode ** rs) {
  if (head == NULL) return 0;
  else {
    int t = LastNth(head->next, n, rs) + 1;
    if (t == n) *rs = head;
    return t;
  }
}

ListNode * LastNth2(ListNode * head, int n) {
  ListNode * rs = NULL;
  LastNth(head, n, &rs);
  return rs;
}


ListNode * LastNth(ListNode * head, int n) {
  n--;
  ListNode * t = head;
  while (t != NULL && n--) t = t->next;
  if (t == NULL) return NULL;
  while (t->next != NULL) {  // bug fix: change t = t->next
    t = t->next;
    head = head->next;
  }
  return head;
}


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  ListNode * head = new ListNode(10);
  LOG(INFO) << LastNth(head, 2);
  LOG(INFO) << LastNth2(head, 2);
  return 0;
}
