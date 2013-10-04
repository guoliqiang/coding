// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 14:33:59
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

struct ListNode {
  int val;
  ListNode * next;
  ListNode (int v = 0) : val(v), next(NULL) {}
};

bool RemoveMiddleNode(ListNode * remove) {
  if (remove->next != NULL) {
    remove->val = remove->next->val;
    remove->next = remove->next->next;
    return true;
  }
  return false;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
