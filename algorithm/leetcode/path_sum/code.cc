// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 21:48:42
// File  : code.cc
// Brief :

/*
Given a binary tree and a sum, determine if the tree has a root-to-leaf path such that adding up all the values along the path equals the given sum.

For example:
Given the below binary tree and sum = 22,
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \      \
        7    2      1
return true, as there exist a root-to-leaf path 5->4->11->2 which sum is 22.

*/

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

namespace twice {
bool HasPathSum(TreeNode * root, int sum, int tsum) {
  if (root == NULL) return false;
  tsum += root->val;
  if (tsum == sum && root->left == NULL && root->right == NULL) return true;
  if (HasPathSum(root->left, sum, tsum)) return true;
  if (HasPathSum(root->right, sum, tsum)) return true;
  return false;
}

bool HasPathSum(TreeNode * root, int sum) {
  int tsum = 0;
  return HasPathSum(root, sum, tsum);
}
}  // namespace twice



int main(int argc, char** argv) {

  return 0;
}
