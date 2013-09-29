// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 20:14:07
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 12/12 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 168 milli secs
 * Progress: 32/32 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x): val(x), next(NULL) {}

};

void SortedListToBST(ListNode * head, int n, TreeNode ** p) {
  if (n <= 0 || head == NULL) return;
  int t = n / 2;
  ListNode * foo = head;
  int i = t;
  while (i > 0) {
    foo = foo->next;
    i--;
  }

  // LOG(INFO) << "n:" << n << " t:" << t << " head:" << head->val;

  *p = new TreeNode(foo->val);
  SortedListToBST(head, t, &((*p)->left));
  SortedListToBST(foo->next, n - t - 1, &((*p)->right));
}

TreeNode *  SortedListToBST(ListNode * head) {
   TreeNode * root = NULL;
   if (head == NULL) return root;
   int n = 0;
   ListNode * foo = head;
   while (foo != NULL) {
     foo = foo->next;
     n++;
   }
   SortedListToBST(head, n, &root);
   return root;
}


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  ListNode * head = new ListNode(3);
  head->next = new ListNode(5);
  head->next->next = new ListNode(8);

  SortedListToBST(head);

  return 0;
}
