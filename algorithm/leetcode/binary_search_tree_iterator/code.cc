// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 11:26:20
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class BSTIterator {
 public:
  BSTIterator(TreeNode *root) {
    while (root != NULL) {
      stack_.push(root);
      root = root->left;
    }
  }

  // @return whether we have a next smallest number
  bool hasNext() {
    return stack_.size() > 0;
  }

  // @return the next smallest number
  int next() {
    TreeNode * cur = stack_.top();
    stack_.pop();

    TreeNode * cur_right = cur->right;
    while (cur_right != NULL) {
      stack_.push(cur_right);
      cur_right = cur_right->left;
    }
    return cur->val;
  }

 private:
  std::stack<TreeNode *> stack_;
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
