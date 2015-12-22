// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 12:49:53
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

ListNode* ReverseList(ListNode* head) {
  ListNode * ans = NULL;
  while (head != NULL) {
    ListNode * next = head->next;
    head->next = ans;
    ans = head;
    head = next;
  }
  return ans;
}

void ReverseListSub(ListNode* head, ListNode ** head2) {
  if (head == NULL) return;
  ListNode * next = head->next;
  head->next = *head2;
  *head2 = head;
  ReverseListSub(next, head2);
}

ListNode* ReverseList2(ListNode* head) {
  ListNode * ans = NULL;
  ReverseListSub(head, &ans);
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
