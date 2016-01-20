// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 15:39:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

namespace algorithm {
// 先根后根都可以
TreeNode * InvertTree(TreeNode* root) {
  if (root == NULL) return NULL;
  InvertTree(root->left);
  InvertTree(root->right);
  std::swap(root->left, root->right);
  return root;
}

}  // namespace algorithm

using namespace algorithm;

namespace Iterator {
TreeNode* invertTree(TreeNode* root) {
  TreeNode * ans = root;
  std::stack<TreeNode *> stack;
  while (root != NULL) {
    stack.push(root);
    std::swap(root->left, root->right);
    root = root->left;
  }
  while (stack.size()) {
    TreeNode * cur = stack.top();
    stack.pop();

    cur = cur->right;
    while (cur != NULL) {
      stack.push(cur);
      std::swap(cur->left, cur->right);
      cur = cur->left;
    }
  }
  return ans;
}
}  // namespace Iterator

int main(int argc, char** argv) {
  return 0;
}
