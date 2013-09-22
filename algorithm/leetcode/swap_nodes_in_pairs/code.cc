// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-22 19:38:44
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 6/6 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 24 milli secs
 * Progress: 55/55 test cases passed.
 * */

#include <vector>
#include <iostream>
#include "base/public/logging.h"

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

bool LetterThanTwo(ListNode * head, ListNode ** ptr) {
  if (head == NULL || head->next == NULL) {
    *ptr = head;
    return true;
  }
  if (head->next->next == NULL) {
    head->next->next = head;
    *ptr = head->next;
    head->next = NULL;
    return true;
  }
  return false;
}


ListNode * SwapPairs(ListNode * head) {
  ListNode * foo = NULL;
  if (LetterThanTwo(head, &foo)) return foo;
  foo = head->next->next;
  head->next->next = head;
  ListNode * pre = head->next;
  head->next = foo;
  head = pre;
  pre = head->next;

  while (foo != NULL) {
    ListNode * swap1 = foo;
    ListNode * swap2 = foo->next;
    if (swap2 == NULL) {
      HERE(INFO);
      LOG(INFO) << foo->val;
      break;
    }
    foo = foo->next->next;

    pre->next = swap2;
    swap2->next = swap1;
    swap1->next = foo;
    pre = pre->next->next;
  }
  return head;
}


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  
  foo.clear();
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(3);
  Out(SwapPairs(MakeList(foo)));

  return 0;
  foo.clear();
  foo.push_back(1);
  Out(SwapPairs(MakeList(foo)));

  foo.clear();
  foo.push_back(1);
  foo.push_back(23);
  Out(SwapPairs(MakeList(foo)));


  foo.clear();
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(3);
  foo.push_back(4);
  foo.push_back(5);
  foo.push_back(6);
  foo.push_back(7);
  Out(SwapPairs(MakeList(foo)));
  return 0;
}
