// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 10:14:31
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

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
