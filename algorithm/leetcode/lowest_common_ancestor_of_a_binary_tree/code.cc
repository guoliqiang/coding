// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 18:03:19
// File  : code.cc
// Brief :
// https://leetcode.com/discuss/45386/4-lines-c-java-python-ruby

#include "base/public/common_ojhead.h"

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

namespace algorithm {
void LCA(TreeNode * root, int * flag, TreeNode * * ans,
         TreeNode * p, TreeNode * q) {
  if (root == NULL) return;
  if (*flag == 2) return;

  int cur = *flag;
  LCA(root->left, flag, ans, p, q);
  if (*flag == 2) return;

  LCA(root->right, flag, ans, p, q);
  if (root == p) (*flag)++;
  if (root == q) (*flag)++;
  if (cur == 0 && *flag == 2 && *ans == NULL) {
    *ans = root;
  }
}

TreeNode* LowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
  int flag = 0;
  TreeNode * ans = NULL;
  LCA(root, &flag, &ans, p, q);
  return ans;

}
}  // namespace algorithm

namespace NB {
TreeNode * LCA(TreeNode * root, TreeNode * p, TreeNode * q) {
    if (root == NULL || p == NULL || q == NULL) return NULL;

    if (root == p || root == q) return root;
    TreeNode * l = LCA(root->left, p, q);
    TreeNode * r = LCA(root->right, p, q);
    if (l != NULL && r != NULL) return root;
    return l != NULL ? l : r;
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  TreeNode * root = new TreeNode(2);
  root->left = new TreeNode(1);
  LowestCommonAncestor(root, root, root->left);
  return 0;
}
