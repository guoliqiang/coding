// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 22:02:22
// File  : code.cc
// Brief :

/*
Given a binary tree where all the right nodes are either leaf nodes with a sibling (a left node that shares the same parent node) or empty, flip it upside down and turn it into a tree where the original right nodes turned into left leaf nodes. Return the new root.
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
};

// 没左子树，必然没有右子树
TreeNode * Get(TreeNode * root, TreeNode * & ans) {
  if (root == NULL) return NULL;
  if (root->left == NULL) {
    ans = root;
  } else {
    TreeNode * l = Get(root->left, ans);
    l->left = root->right;
    l->right = root;
    root->left = NULL;
    root->right = NULL;
  }
  return root;
}

TreeNode * UpsideDown(TreeNode * root) {
  TreeNode * ans = NULL;
  Get(root, ans);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
