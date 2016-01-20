// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 18:03:19
// File  : code.cc
// Brief :
// https://www.hikyle.me/archives/957/
// http://www.acmerblog.com/lca-lowest-common-ancestor-5574.html
// https://leetcode.com/discuss/44959/3-lines-with-o-1-space-1-liners-alternatives

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

TreeNode * LCAIte(TreeNode * root, TreeNode * p, TreeNode * q) {
  if (root == NULL || p == NULL || q == NULL) return NULL;

  while (true) {
    if (p->val > root->val && q->val > root->val) {
      root = root->right;
    } else if (p->val < root->val && q->val < root->val) {
      root = root->left;
    } else {
      return root;
    }
  }
  return NULL;
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
