// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 10:14:31
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

namespace algorithm {
std::string ToString(int v) {
  char buff[100] = {0};
  sprintf(buff, "%d", v);
  return std::string(buff);
}

void PreOrder(TreeNode * root, std::vector<int> & path,
              std::vector<std::string> & ans) {
  if (root == NULL) return;

  path.push_back(root->val);
  if (root->left == NULL && root->right == NULL) {
    std::string rs;
    for (int i = 0; i < path.size(); i++) {
      if (i == 0) rs = ToString(path[i]);
      else rs += ("->" + ToString(path[i]));
    }
    ans.push_back(rs);
  }
  PreOrder(root->left, path, ans);
  PreOrder(root->right, path, ans);
  path.pop_back();
}

std::vector<std::string> BinaryTreePaths(TreeNode* root) {
  std::vector<std::string> ans;
  std::vector<int> path;
  PreOrder(root, path, ans);
  return ans;
}
}  // namespace algorithm

namespace iter {
std::string IntToString(int v) {
    char buff[100] = { 0 };
    sprintf(buff, "%d", v);
    return std::string(buff);
}

std::vector<std::string> BinaryTreePaths(TreeNode* root) {
  std::vector<std::string> ans;
  std::vector<std::pair<TreeNode *, int> > stack;
  while (root != NULL) {
      stack.push_back(std::make_pair(root, 0));
      root = root->left;
  }

  while (stack.size()) {
    std::pair<TreeNode *, int> cur = stack.back();
    if (cur.second == 1) {
      if (cur.first->left == NULL && cur.first->right == NULL) {
          std::string one;
          for (int i = 0; i < stack.size(); i++) {
              one += IntToString(stack[i].first->val) + "->";
          }
          ans.push_back(one.substr(0, one.size() - 2));
      }
      stack.pop_back();
    } else {
      stack.back().second = 1;
      TreeNode * t = cur.first->right;
      while (t != NULL) {
          stack.push_back(std::make_pair(t, 0));
          t = t->left;
      }
    }
  }
  return ans;
}
}  // namespace iter

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
