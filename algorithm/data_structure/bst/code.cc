// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-01 15:57:37
// File  : code.cc
// Brief :

/*
bst insert find delete
http://en.wikipedia.org/wiki/Binary_search_tree
http://zh.wikipedia.org/wiki/%E4%BA%8C%E5%85%83%E6%90%9C%E5%B0%8B%E6%A8%B9
trick:delete 操作
*/
#include "base/public/common_ojhead.h"

namespace algorithm {

struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
  TreeNode(int v) : val(v){}
};

TreeNode * Find(TreeNode * root, int t) {
  if (root == NULL) return NULL;
  if (root->val == t) return root;
  else if (root->val > t) return Find(root->left, t);
  else return Find(root->right, t);
}

TreeNode * FindParent(TreeNode * root, int t) {
  if (root == NULL) return NULL;
  if ((root->left != NULL && root->left->val == t) ||
      (root->right != NULL && root->right->val == t)) return root;
  if (root->val > t) return FindParent(root->left, t);
  else if (root->val < t) return FindParent(root->right, t);
  else return NULL;
}

void InsertInternal(TreeNode * root, int t) {
  if (root->val > t) {
    if (root->left == NULL) root->left = new TreeNode(t);
    else InsertInternal(root->left, t);
  } else {
    if (root->right == NULL) root->right = new TreeNode(t);
    else InsertInternal(root->right, t);
  }
}

bool Insert(TreeNode * & root, int t) {
  if (Find(root, t) == NULL) return false;
  if (root == NULL) root = new TreeNode(t);
  else Insert(root, t);
  return true;
}

void DeleteInternal(TreeNode * p, TreeNode * & root) {
  if (p->left == NULL && p->right == NULL) {
    TreeNode * parent = FindParent(root, p->val);
    if (parent == NULL) {
      root = NULL;
    } else {
      if (parent->left == p) parent->left = NULL;
      else parent->right = NULL;
    }
    delete p;
  } else if (p->left == NULL) {
    TreeNode * d = p->right;
    p->val = d->val;
    p->left = d->left;
    p->right = d->right;
    delete d;
  } else if (p->right == NULL) {
    TreeNode * d = p->left;
    p->val = d->val;
    p->left = d->left;
    p->right = d->right;
    delete d;
  } else {
    TreeNode * pre = p->left;
    TreeNode * cur = p->left;
    while (cur->right != NULL) {
      pre = cur;
      cur = cur->right;
    }
    p->val = cur->val;
    if (pre == cur) {
      p->left = cur->left;
    } else  {
      pre->right = cur->left;
    }
    delete cur;
  }
}

bool Delete(TreeNode * & root, int t) {
  TreeNode * d = Find(root, t);
  if (d == NULL) return false;
  DeleteInternal(d, root);
  return true;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
