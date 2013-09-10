// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 20:33:55
// File  : bst_iamge.h
// Brief :

#ifndef  __BST_IAMGE_H_
#define  __BST_IAMGE_H_

#include "../bst_to_dq/bst.h"

namespace algorithm {

template <typename type>
void BSTImage(Node<type> * root) {
  if (root == NULL) return;
  BSTImage(root->left);
  BSTImage(root->right);
  Node<type> * foo = root->left;
  root->left = root->right;
  root->right = foo;
}

}  // namespace algorithm

#endif  // __BST_IAMGE_H_
