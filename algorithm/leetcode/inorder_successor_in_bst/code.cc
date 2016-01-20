// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 11:56:43
// File  : code.cc
// Brief :

// http://www.geeksforgeeks.org/inorder-successor-in-binary-search-tree/

#include "base/public/common_ojhead.h"

namespace algorithm {

struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
};

TreeNode* InorderSuccessor(TreeNode* root, TreeNode* p) {
  TreeNode * pre = NULL;
  TreeNode * cur = p->right;
  while (cur != NULL) {
    pre = cur;
    cur = cur->left;
  }
  if (pre != NULL) return pre;

  while (root->val != p->val) {
    if (root->val > p->val) {
      pre = root;
      root = root->left;
    } else {
      root = root->right;
    }
  }
  return pre;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
