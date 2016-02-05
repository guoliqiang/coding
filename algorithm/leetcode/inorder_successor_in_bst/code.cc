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

TreeNode * PreorderSuccessor(TreeNode* root, TreeNode* p) {
  if (p->left != NULL) return p->left;
  else if (p->right != NULL) return p->right;
  else {
    TreeNode * pre = NULL;
    while (root != p) {
      if (root->val > p->val) {
        pre = root;
        root = root->left;
      } else {
        root = root->right;
      }
    }
    return pre == NULL ? NULL : pre->right;
  }
}

TreeNode * PostorderSuccessor(TreeNode * root, TreeNode * p) {
  TreeNode * parent = NULL;
  while (root != p) {
    parent = root;
    if (root->val > p->val) root = root->left;
    else root = root->right;
  }
  if (parent == NULL) return NULL;

  if (parent->right == p || parent->right == NULL) return parent;
  else {
    TreeNode * ans = parent->right;
    while (ans->left != NULL) ans = ans->left;
    return ans;
  }
}

TreeNode* InorderPrecessor(TreeNode* root, TreeNode* p) {
  // 小于p最大的那个数
  if (p->left == NULL) return NULL;
  else {
    TreeNode * t = p->left;
    while (t->right != NULL) t = t->right;
    return t;
  }
}

TreeNode* PrePrecessor(TreeNode* root, TreeNode* p) {
  TreeNode * parent = NULL;
  while (root != p) {
    parent = root;
    if (root->val > p->val) root = root->left;
    else root = root->right;
  }

  if (parent->left == p || p->left == NULL) {
    return parent;
  } else {
    parent = parent->left;
    while (parent->right != NULL) parent = parent->right;
    return parent;
  }
}

TreeNode* PostPrecessor(TreeNode* root, TreeNode* p) {
  if (root->left != NULL) {
    root = root->left;
    while (root->left != NULL) root = root->left;
    return root;
  } else if (root->right != NULL) {
    root = root->right;
    while (root->left != NULL) root = root->left;
    return root;
  } else {
    return NULL;
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
