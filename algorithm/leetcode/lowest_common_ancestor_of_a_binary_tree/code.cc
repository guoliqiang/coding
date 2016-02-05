// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 18:03:19
// File  : code.cc
// Brief :
// https://leetcode.com/discuss/45386/4-lines-c-java-python-ruby

#include "base/public/common_ojhead.h"

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

namespace algorithm {
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

namespace NB {
TreeNode * LCA(TreeNode * root, TreeNode * p, TreeNode * q) {
    if (root == NULL || p == NULL || q == NULL) return NULL;

    if (root == p || root == q) return root;
    TreeNode * l = LCA(root->left, p, q);
    TreeNode * r = LCA(root->right, p, q);
    if (l != NULL && r != NULL) return root;
    return l != NULL ? l : r;
}
}  // namespace NB

namespace iter {

// 树上找path只能用后跟遍历，注意后跟遍历在第二次处栈的时候，不要犯了重复访问
// 右子树的错误
TreeNode* LowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
  std::vector<std::vector<TreeNode *> > paths;
  std::vector<TreeNode *> stack;
  std::vector<int> sign;
  while (root != NULL) {
    stack.push_back(root);
    sign.push_back(0);
    root = root->left;
  }
  while (stack.size()) {
    TreeNode * cur = stack.back();
    if (sign.back() == 1) {
      if (cur == p || cur == q) {
        paths.push_back(stack);
      }
      sign.pop_back();
      stack.pop_back();
    } else {
      sign.back() = 1;
      cur = cur->right;
      while (cur != NULL) {
        stack.push_back(cur);
        sign.push_back(0);
        cur = cur->left;
      }
    }
  }
  if (paths.size() == 1) return paths[0].back();

  if (paths[0].size() > paths[1].size()) std::swap(paths[0], paths[1]);
  while (paths[1].size() > paths[0].size()) paths[1].pop_back();

  while (paths[0].back() != paths[1].back()) {
    paths[0].pop_back();
    paths[1].pop_back();
  }
  return paths[0].back();
}
}  // namespace iter

using namespace algorithm;

int main(int argc, char** argv) {
  TreeNode * root = new TreeNode(3);
  root->right = new TreeNode(1);
  root->right->left = new TreeNode(0);
  root->right->right = new TreeNode(8);
  iter::LowestCommonAncestor(root, root->right->left, root->right->right);
  return 0;
}
