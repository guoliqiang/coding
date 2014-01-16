// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-14 17:46:53
// File  : code.cc
// Brief :
// http://www.geeksforgeeks.org/check-leaves-level/

#include "base/public/common_ojhead.h"

namespace algorithm {
struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
  TreeNode(int v = 0) : val(v), left(NULL), right(NULL) {}
};

bool PreOrder(TreeNode * root, int k, int & level) {
  if (root == NULL) return true;
  if (root->left == NULL && root->right == NULL) {
    if (level == -1) level = k;
    if (level != k) return false;
  }
  if (root->left != NULL && PreOrder(root->left, k + 1, level) == false) return false;
  if (root->right != NULL && PreOrder(root->right, k + 1, level) == false) return false;
  return true;
}

bool SamleLevel(TreeNode * root) {
  int level = -1;
  return PreOrder(root, 0, level);
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
