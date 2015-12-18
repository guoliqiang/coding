// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-18 15:52:37
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

ListNode* RemoveElements(ListNode* head, int val) {
  while (head != NULL && head->val == val) {
    head = head->next;
  }
  ListNode * ans = head;
  ListNode * before = NULL;
  while (head != NULL) {
    if(head->val == val) {
      before->next = head->next;
    } else {
      before = head;
    }
    head = head->next;
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
