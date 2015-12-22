// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 18:47:17
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

bool IsPalindrome(ListNode* head) {
  int len = 0;
  ListNode * cur = head;
  while (cur != NULL) {
    len++;
    cur = cur->next;
  }
  if (len == 0) return true;
  int half = len / 2;
  if (len % 2 == 1) half += 1;

  cur = head;
  for (int i = 0; i < half; i++) cur = cur->next;
  ListNode * tail = NULL;
  while (cur != NULL) {
    ListNode * next = cur->next;
    cur->next = tail;
    tail = cur;
    cur = next;
  }

  cur = head;
  for (int i = 0; i < len / 2; i++) {
    if (cur->val != tail->val) return false;
    cur = cur->next;
    tail = tail->next;
  }
  return true;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
