// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-08 22:59:20
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
  TreeNode(int v) : val(v), left(NULL), right(NULL) {}
};

namespace preorder {
void Get(TreeNode * root, TreeNode * pre, int cur, int * ans) {
  if (pre != NULL && root->val == pre->val + 1) {
    cur += 1;
  } else cur = 1;

  *ans = std::max(*ans, cur);
  Get(root->left, root, cur, ans);
  Get(root->right, root, cur, ans);
}
}

namespace algorithm {
int Get(TreeNode * root, int * ans) {
  if (root == NULL) return 0;

  int cur = 1;
  int l = Get(root->left, ans);
  int r = Get(root->right, ans);
  if (root->left != NULL && root->left->val - 1 == root->val) {
    cur = std::max(cur, cur + l);
  }
  if (root->right != NULL && root->right->val - 1 == root->val) {
    cur = std::max(cur, cur + r);
  }
  *ans = std::max(*ans, cur);
  return cur;
}

int LongestConsecutive(TreeNode* root) {
  int ans = 0;
  Get(root, &ans);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  TreeNode * root = new TreeNode(2);
  TreeNode * cur = root;
  cur->right = new TreeNode(3);
  cur = cur->right;
  cur->left = new TreeNode(2);
  cur = cur->left;
  cur->left = new TreeNode(1);

  LOG(INFO) << LongestConsecutive(root);
  return 0;
}
