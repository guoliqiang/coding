// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-05 21:50:59
// File  : code.cc
// Brief :

/*
Given a binary tree, return the preorder traversal of its nodes' values.
For example:
Given binary tree {1,#,2,3},
   1
    \
     2
    /
   3
return [1,2,3].
Note: Recursive solution is trivial, could you do it iteratively?
*/

#include "base/public/common_head.h"
#include "../ojbst/ojbst.h"

/*
 *
 * 67 / 67 test cases passed.
 * Status: Accepted
 * Runtime: 24 ms
 * Submitted: 0 minutes ago
 * 
 * 线索二叉树
 * */

namespace algorithm {

std::vector<int> PreOrderTra(TreeNode * root) {
  std::vector<int> rs;
  while (root != NULL) {
    if (root->left == NULL) {
      rs.push_back(root->val);
      root = root->right;
    } else {
      TreeNode * t = root->left;
      while (t->right != NULL && t->right != root) {
        t = t->right;
      }
      if (t->right == NULL) {
        rs.push_back(root->val);
        t->right = root;
        root = root->left;
      } else {
        t->right = NULL;
        root = root->right;
      }
    }
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
std::vector<int> PreOrderTra(TreeNode * root) {
  std::vector<int> rs;
  std::stack<TreeNode *> stack;
  while (root != NULL) {
    rs.push_back(root->val);
    stack.push(root);
    root = root->left;
  }
  while (!stack.empty()) {
    TreeNode * foo = stack.top()->right;
    stack.pop();
    while (foo != NULL) {
      rs.push_back(foo->val);
      stack.push(foo);
      foo = foo->left;
    }
  }
  return rs;
}
}  // namespace twice

int main(int argc, char** argv) {

  return 0;
}
