// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-16 20:25:19
// File  : balance_tree.h
// Brief :

#ifndef  __BALANCE_TREE_H_
#define  __BALANCE_TREE_H_

#include "base/public/logging.h"
#include "../bst_to_dq/bst.h"
#include "stdlib.h"
#include "math.h"

namespace algorithm {

// 后根遍历，每个点只可能访问一次
template <typename type>
bool BalanceTree(Node<type> * root, int * deepth) {
  if (root == NULL)  {
    *deepth = 0;
    return true;
  }
  int left = 0;
  if (!BalanceTree(root->left, &left)) return false;
  
  int right = 0;
  if (!BalanceTree(root->left, &right)) return false;
  *deepth = (left > right ? left: right) + 1;

  return fabs(left - right) <= 1;
  
}

template <typename type>
bool BalanceTree(Node<type> * root) {
  int deepth = 0;
  return BalanceTree(root, &deepth);
}
}  // namespace algorithm

#endif  //  __BALANCE_TREE_H_
