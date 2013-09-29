// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 17:30:48
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 28/28 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 36 milli secs
 * Progress: 190/190 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

bool IsSymmetric(TreeNode * root1, TreeNode * root2) {
  if (root1 == NULL && root2 == NULL) return true;
  if (root1 == NULL || root2 == NULL) return false;

  if (root1->val != root2->val) return false;
  if (IsSymmetric(root1->left, root2->right) &&
      IsSymmetric(root1->right, root2->left)) return true;
  return false;
}

bool IsSymmetric(TreeNode * root) {
  if (root == NULL) return true;
  return IsSymmetric(root->left, root->right);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
