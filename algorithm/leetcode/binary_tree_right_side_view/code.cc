// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-17 23:11:11
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

std::vector<int> RightSideView(TreeNode* root) {
  std::vector<int> ans;
  if (root == NULL) return ans;
  std::list<TreeNode *> queue;
  queue.push_back(root);
  queue.push_back(NULL);

  TreeNode * before = NULL;
  while(queue.size() > 0) {
    TreeNode * cur = queue.front();
    queue.pop_front();
    if (cur == NULL && queue.size() == 0) {
      break;
    } else if (cur == NULL) {
      ans.push_back(before->val);
      queue.push_back(NULL);
    } else {
      if (cur->left != NULL) queue.push_back(cur->left);
      if (cur->right != NULL) queue.push_back(cur->right);
      before = cur;
    }
  }
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
