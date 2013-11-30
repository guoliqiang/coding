// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 18:36:20
// File  : code.cc
// Brief :

/*
Given a binary tree, find its maximum depth.

The maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 12/12 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 48 milli secs
 * Progress: 38/38 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

int MaxDepth(TreeNode * root) {
  if (root == NULL) return 0;
  int l = MaxDepth(root->left);
  int r = MaxDepth(root->right);
  return l > r ? l + 1 : r + 1;
}

}  // namespace algorithm


int main(int argc, char** argv) {

  return 0;
}
