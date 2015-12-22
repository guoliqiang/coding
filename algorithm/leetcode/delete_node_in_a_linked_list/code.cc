// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 17:34:06
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

void DeleteNode(ListNode* node) {
  while(node->next->next != NULL) {
    node->val = node->next->val;
    node = node->next;
  }
  node->val = node->next->val;
  node->next = NULL;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
