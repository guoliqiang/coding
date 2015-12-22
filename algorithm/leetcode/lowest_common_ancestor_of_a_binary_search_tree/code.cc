// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 18:03:19
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

TreeNode * LCANB(TreeNode * root, TreeNode * p, TreeNode * q) {
  if (root == NULL) return NULL;
  if ((root->val > p->val) && (root->val > q->val)) {
    return LCANB(root->left, p, q);
  } else if ((root->val < p->val) && (root->val < q->val)) {
    return LCANB(root->right, p, q);
  } else return root;
}

void LCA(TreeNode * root, int * flag, TreeNode * * ans,
         TreeNode * p, TreeNode * q) {
  if (root == NULL) return;
  if (*flag == 2) return;

  int cur = *flag;
  LCA(root->left, flag, ans, p, q);
  if (*flag == 2) return;

  LCA(root->right, flag, ans, p, q);
  if (root == p) (*flag)++;
  if (root == q) (*flag)++;
  LOG(INFO) << cur << " " << *flag;
  if (cur == 0 && *flag == 2 && *ans == NULL) {
    *ans = root;
  }
}

TreeNode* LowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
  int flag = 0;
  TreeNode * ans = NULL;
  LCA(root, &flag, &ans, p, q);
  return ans;

}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  TreeNode * root = new TreeNode(2);
  root->left = new TreeNode(1);
  LowestCommonAncestor(root, root, root->left);
  return 0;
}
