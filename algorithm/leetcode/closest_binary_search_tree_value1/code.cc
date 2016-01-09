// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 23:13:46
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
};

int ClosestValue(TreeNode * root, double target) {
  TreeNode * ans = NULL;
  while (root != NULL) {
    if (ans == NULL) ans = root;
    else if (fabs(root->val - target) < fabs(ans->val - target)) {
      ans = root;
    }

    if (root->val > target) root = root->left;
    else root = root->right;
  }
  return ans == NULL ? -1 : ans->val;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  return 0;
}
