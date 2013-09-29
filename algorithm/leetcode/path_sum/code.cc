// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 21:48:42
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 43/43 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 64 milli secs
 * Progress: 114/114 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

bool HasPathSum(TreeNode * root, int sum, int * tsum) {
  if (root == NULL) return false;
  *tsum += root->val;
  if (root->left == NULL && root->right == NULL && *tsum == sum) return true;
  if (HasPathSum(root->left, sum, tsum)) return true;
  if (HasPathSum(root->right, sum, tsum)) return true;
  *tsum -= root->val;
  return false;
}

bool HasPathSum(TreeNode * root, int sum) {
  if (root == NULL) return false;
  int tsum = 0;
  return HasPathSum(root, sum, &tsum);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
