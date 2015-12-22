// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 21:40:03
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

void InOrder(TreeNode * root, int k, int * rank, int * ans) {
  if (root == NULL) return;
  InOrder(root->left, k, rank, ans);
  (*rank) += 1;
  if ((*rank) == k) *ans = root->val;
  InOrder(root->right, k, rank, ans);
}

int KthSmallest(TreeNode* root, int k) {
  int rank = 0;
  int ans = 0;
  InOrder(root, k, &rank, &ans);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  return 0;
}
