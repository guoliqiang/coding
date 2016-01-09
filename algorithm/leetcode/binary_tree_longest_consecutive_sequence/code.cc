// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-08 22:59:20
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
  TreeNode(int v) : val(v), left(NULL), right(NULL) {}
};

int Get(TreeNode * root, int * ans) {
  if (root == NULL) return 0;

  int cur = 1;
  if (root->left != NULL && root->left->val == root->val + 1) {
    cur = std::max(cur, 1 + Get(root->left, ans));
  }
  if (root->right != NULL && root->right->val == root->val + 1) {
    cur = std::max(cur, 1 + Get(root->right, ans));
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
