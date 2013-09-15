// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 09:38:40
// File  : sub_tree.h
// Brief :

#ifndef  __SUB_TREE_H_
#define  __SUB_TREE_H_

#include "../bst_to_dq/bst.h"

namespace algorithm {
template <typename type>
bool SubTreeSub(Node<type> * root1, Node<type> * root2) {
  if (root1 == NULL && root2 != NULL) return false;
  if (root1 != NULL && root2 == NULL) return true;
  if (root1 == NULL && root2 == NULL) return true;
  if (root1->value != root2->value) return false;
  bool rs =  (SubTreeSub(root1->left, root2->left) &&
              SubTreeSub(root1->right, root2->right));
  return rs;
}

template <typename type>
bool SubTree(Node<type> * root1, Node<type> * root2) {
  if (SubTreeSub(root1, root2)) return true;
  if (root1 != NULL) {
    if (SubTree(root1->left, root2)) return true;
    if (SubTree(root1->right, root2)) return true;
  }
  return false;
}

}  // namespace algorithm



#endif  //__SUB_TREE_H_
