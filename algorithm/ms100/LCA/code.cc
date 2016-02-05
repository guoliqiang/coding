// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-09 04:49:13
// File  : code.cc
// Brief :


#include "base/public/common_ojhead.h"

namespace algorithm {
struct TreeNode{
  int val;
  TreeNode * left;
  TreeNode * right;
};

void LCA(std::map<int, TreeNode *> & v, int deep,
         int x, int y, int & flag, TreeNode * root) {
  if (root == NULL || flag == 2) return;
  if (root->val == x) {
    flag++;
    v.insert(std::make_pair(deep, root));
  }
  if (root->val == y) {
    flag++;
    v.insert(std::make_pair(deep, root));
  }
  if (flag == 2) return;
  LCA(v, deep + 1, x, y, flag, root->left);
  if (flag == 2) return;
  if (flag == 1) v.insert(std::make_pair(deep, root));
  
  LCA(v, deep + 1, x, y, flag, root->right);
  if (flag == 1) v.insert(std::make_pair(deep, root));
}

TreeNode * LCA(int x, int y, TreeNode * root) {
  std::map<int, TreeNode *> v;
  int deep = 0;
  int flag = 0;
  LCA(v, deep, x, y, flag, root);
  return v.size() > 0 ? v.begin()->second : NULL;
}

} // namespace algorithm

namespace NB {
// for bst
TreeNode * LCA(TreeNode * root, int x, int y) {
  if (root == NULL) return NULL;
  if (x > root->val && y > root->val) return LCA(root->right, x, y);
  if (x < root->val && y < root->val) return LCA(root->left, x, y);
  return root;
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
