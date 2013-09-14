// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 13:13:07
// File  : complex_list.h
// Brief :

#ifndef  __COMPLEX_LIST_H_
#define  __COMPLEX_LIST_H_

#include <vector>
#include "base/public/logging.h"
#include "stdlib.h"
#include <algorithm>

namespace algorithm {

template <typename type>
struct CNode {
  type value;
  CNode<type> * next;
  CNode<type> * sibling;
};

template <typename type>
int Size(CNode<type> * head) {
  int rs = 0;
  while (head) {
    rs++;
    head = head->next;
  }
  return rs;
}

template <typename type>
CNode<type> * Move(CNode<type> * h, int i) {
  while (i) {
    h = h->next;
    i--;
  }
  return h;
}

template <typename type>
CNode<type> * BuildCList(std::vector<type> & v) {
  CNode<type> * head = NULL;
  CNode<type> * before = NULL;
  for (int i = 0; i < v.size(); i++) {
    if (i == 0) {
      head = new CNode<type>();
      head->value = v[i];
      before = head;
    } else {
      CNode<type> * foo = new CNode<type>();
      foo->value = v[i];
      before->next = foo;
      before = foo;
    }
  }
  CNode<type> * foo = head;
  while (foo) {
    foo->sibling = Move(head, random() % Size(head));
    foo = foo->next;
  }
  return head;
}

template <typename type>
void OutCList(CNode<type> * h) {
  while (h) {
    LOG(INFO) << h->value;
    if (h->sibling) LOG(INFO) << "\t" << h->sibling->value;
    h = h->next;
  }
}

// A->B->C->D
// |     |
// ------
//
// A->a->B->b->C->c
// |           |
// ------------
//
// A->a->B->b->C->c
// |  |        |  |
// |  ---------|---
// -------------

template <typename type>
CNode<type> * CopyCList(CNode<type> * h) {
  CNode<type> * cp_h = h;
  while (h) {
    CNode<type> * foo = new CNode<type>();
    foo->value = h->value;
    CNode<type> * bar = h->next;
    h->next = foo;
    foo->next = bar;
    h = bar;
  }  // copy value;
  HERE(INFO);
  h = cp_h;
  while (h) {
    if (h->sibling) {
      h->next->sibling = h->sibling->next;
    } else {
      h->next->sibling = NULL;
    }
    h = h->next->next;
  }  // copy sibling
  HERE(INFO);
  h = cp_h;
  CNode<type> * head = NULL;
  CNode<type> * before = NULL;
  while (h) {
    if (head == NULL) {
      head = h->next;
      before = head;
      h->next = h->next->next;
    } else {
      before->next = h->next;
      before = h->next;
      h->next = h->next->next;
    }
    h = h->next;
  }  // cut
  return head;
}

template <typename type>
void Delete(CNode<type> * h) {
  while (h) {
    CNode<type> * foo = h;
    h = h->next;
    delete foo;
  }
}

}  // namespace algorithm


#endif  //__COMPLEX_LIST_H_
