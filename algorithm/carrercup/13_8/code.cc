// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-07 00:24:57
// File  : code.cc
// Brief :

/*
 *  Write a method that takes a pointer to a Node structure as a parameter and
 *  returns a complete copy of the passed-in data structure.
 *  The Node structure contains two pointers to other Node structures.
 * */

#include "base/public/common_head.h"

namespace algorithm {

struct Node {
  int val;
  Node * a1;
  Node * a2;
  Node(int v) : val(v), a1(NULL), a2(NULL) {}
};

void Copy(Node * root, Node ** p) {
  if (root == NULL) return;
  *p = new Node(root->val);
  Copy(root->a1, &((*p)->a1));
  Copy(root->a2, &((*p)->a2));
}


Node * Copy(Node * root) {
  Node * rs = NULL;
  Copy(root, &rs);
  return rs;
}

Node * Copy2(Node * root) {
  if (root == NULL) return NULL;
  Node * ptr = new Node(root->val);
  ptr->a1 = Copy2(root->a1);
  ptr->a2 = Copy2(root->a2);
  return ptr;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
