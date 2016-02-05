// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-02-01 22:03:02
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct TreeNode {
  int val;
  TreeNode * parent;
  TreeNode * left;
  TreeNode * right;
  TreeNode() : val(0), parent(NULL), left(NULL), right(NULL) {}
};

TreeNode * Find(TreeNode * root, int v) {
  while (root != NULL) {
    if (root->val == v) return root;
    else if (root->val > v) root = root->left;
    else root = root->right;
  }
  return NULL;
}

TreeNode * Insert(TreeNode * root, int v) {
  if (Find(root, v) != NULL) return root;
  TreeNode * pre = NULL;
  while (root != NULL) {
    pre = root;
    if (root->val > v) root = root->left;
    else root = root->right;
  }
  if (pre == NULL) {
    pre = new TreeNode();
    pre->val = v;
    return pre;
  } else if (pre->val > v) {
    pre->left = new TreeNode();
    pre->left->val = v;
    pre->left->parent = pre;
  } else {
    pre->right = new TreeNode();
    pre->right->val = v;
    pre->right->parent = pre;
  }
  return root;
}

TreeNode * Delete(TreeNode * root, int v) {
  TreeNode * t = Find(root, v);
  if (t == NULL) return root;
  if (t == root) {
    if (t->right == NULL) {
      t->left->parent = NULL;
      return t->left;
    }
    else {
      TreeNode * r = t->right;
      while (r->left != NULL) r = r->left;
      r->left = t->left;
      r->parent->left = NULL;
      r->parent = NULL;
      r->right = t->right;
      return r;
    }
  } else {
    if (t->right == NULL) {
      t->left->parent = t->parent;
      if (t->parent->left == t) t->parent->left = t->left;
      else t->parent->right = t->left;
    } else {
      TreeNode * r = t->right;
      while (r->left != NULL) r = r->left;
      r->parent->left = NULL;
      r->left = t->left;
      r->right = t->right;
      r->parent = t->parent;
      if (t->parent->left == t) t->parent->left = r;
      else t->parent->right = r;
    }
    return root;
  }
}

}  // namespace algorithm


using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
