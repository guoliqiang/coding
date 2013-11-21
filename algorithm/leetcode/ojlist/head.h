// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-21 02:13:32
// File  : head.h
// Brief :

#ifndef  __HEAD_H_
#define  __HEAD_H_

#include "base/public/common_head.h"

struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x = 0) : val(x), next(NULL){}
};

std::string Out(ListNode * pre) {
  std::string rs = "";
  while(pre != NULL) {
    rs += " " + IntToString(pre->val);
    pre = pre->next;
  }
  return rs;
}

ListNode * BuildList(std::vector<int> & v) {
  ListNode * rs = NULL;
  ListNode ** pre = &rs;
  for (int i = 0; i < v.size(); i++) {
    *pre = new ListNode(v[i]);
    pre = &((*pre)->next);
  }
  return rs;
}

#endif  //__HEAD_H_
